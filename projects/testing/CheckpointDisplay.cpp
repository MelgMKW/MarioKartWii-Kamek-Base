#include <Kamek/kamek.hpp>
#include <System/GX.hpp>
#include <System/camera.hpp>
#include <Race/racedata.hpp>
#include <Race/kmp.hpp>
#include <Race/Item/itemholder.hpp>
#include <project.hpp>
#include <ParamsHolder.hpp>
#include <testing.hpp>

//8021ef8c

void SetupGX(s32 zMode, Mtx34 *viewMtx)
{
    // Disable some stuff
    GX::GXSetCullMode(GX_CULL_NONE); // disable all culling
    GX::GXSetZMode(true, GX_LEQUAL,true); // always pass z test
    GX::GXSetZCompLoc(false); // compare z before texture
    GX::GXSetAlphaCompare(GX_ALWAYS, 0x0, GX_AOP_OR, GX_ALWAYS, 0x0);  //fix HP effect
    GX::GXSetBlendMode(GX_BM_BLEND, GX_BL_INVSRCALPHA,GX_BL_INVDSTALPHA, GX_LO_OR); // write output directly to EFB
    GX::GXSetAlphaUpdate(GX_ENABLE);
    GX::GXSetColorUpdate(GX_ENABLE);
    
    u8 RGBA[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    GX::GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, RGBA); // disable fog

    // Set verex input format
    GX::GXClearVtxDesc(); // reset vertex properties
    GX::GXSetVtxDesc(GX_VA_POS, GX_DIRECT); // position coordinates first
    GX::GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT); // colour second
    GX::GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); // position format is a f32 vec3
    GX::GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0); // color format is rgba8

    // Setup for direct colour
    GX::GXSetNumChans(1); // enable 1 colour channel
    GX::GXSetNumTexGens(0); // disable tex coord generation
    GX::GXSetNumTevStages(1); // only 1 TEV stage
    GX::GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0); // disable textures in TEV
    // set colour to only come from the vertices
    GX::GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE); 
    GX::GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR); // set TEV to pass the colour directly

    // Setup view matrix
    GX::GXLoadPosMtxImm(viewMtx, 0);
    GX::GXSetCurrentMtx(0);
}


void DrawRectangleGX(float x1, float x2, float y1, float y2, float z1, float z2, u32 colour){   
    GX::GXBegin(GX_QUADS, 0, 4);
    {
        GX::GX_Position3f32(x1, y2, z1);
        GX::GX_Color1u32(colour);
        GX::GX_Position3f32(x2, y2, z2);
        GX::GX_Color1u32(colour);
        GX::GX_Position3f32(x2, y1, z2);
        GX::GX_Color1u32(colour);
        GX::GX_Position3f32(x1, y1, z1);
        GX::GX_Color1u32(colour);
    }
}

void CircleGX(float x, float y, float z, float radius, u32 pointsNbr, u32 colour){
    float angleStep = PIx2 / (float) pointsNbr;
    GX::GXBegin(GX_LINESTRIP, GX_VTXFMT0, pointsNbr + 2);
    for (float a = 0.0f; a < PIx2; a += angleStep){
        float _x = radius * cos(a);
        float _z = radius * sin(a);
        GX::GX_Position3f32(x + _x, y, z + _z);
        GX::GX_Color1u32(colour);
    }
    GX::GX_Position3f32(x + radius, y, z);
    GX::GX_Color1u32(colour);
    GXEnd();
}

void CylinderBarsGXFilled(float x, float y, float z, float radius, float height, s32 barPointsNbr, u32 colour)
{
    float angleStep = PIx2 / (float) barPointsNbr;
    float _x = radius * cos(PIx2 - angleStep);
    float _z = radius * sin(PIx2 - angleStep);

    for (float a = 0.0f; a < PIx2; a += angleStep){
        float xprev = _x;
        float zprev = _z;
        _x = radius * cos(a);
        _z = radius * sin(a);

        DrawRectangleGX(x + _x, x + xprev, y, y + height, z + _z, z + zprev, colour);
    }
    GXEnd();
}

void CylinderBarsGX(float x, float y, float z, float radius, float height, s32 barPointsNbr, u32 colour)
{
    float angleStep = PIx2 / (float) barPointsNbr;
    GX::GXBegin(GX_LINES, GX_VTXFMT0, barPointsNbr * 2 + 2);
    for (float a = 0.0f; a < PIx2; a += angleStep){
        float _x = radius * cos(a);
        float _z = radius * sin(a);
        GX::GX_Position3f32(x + _x, y, z + _z);
        GX::GX_Color1u32(colour);
        GX::GX_Position3f32(x + _x, y + height, z + _z);
        GX::GX_Color1u32(colour);
    }
    GXEnd();
}

void DrawLineCylinderGX(Mtx34 *viewMtx, float x, float y, float z, float radius, float height, s32 pointsNbr, s32 barPointsNbr, u32 colour){
    SetupGX(GX_LEQUAL, viewMtx);
    //CircleGX(x, y, z, radius, pointsNbr, colour);
    CylinderBarsGXFilled(x, y, z, radius, height, barPointsNbr, colour);
    CircleGX(x, y + height, z, radius, pointsNbr, colour);
}


void DisplayCheckPoints(){
    RaceCameraManager *manager = cameraHandler.cameraArray[0];
    manager->SetProjection(1, 0);
    Mtx34 *worldView = &manager->unkStruct->worldView; //hud slot 0
    KMP::Controller *control = KMP::controller;
    ParamsHolder *params = ParamsHolder::sInstance;

    KMP::ItemPointSectionHolder *itemPointHolder= control->itemPoint;

    if (params->settings.scrollSetting[SCROLLER_INFO] == SCROLLER_INFO_CHECKPOINT){
        KMP::CheckPointSectionHolder *cpHolder = control->checkPoint;
        SetupGX(GX_ALWAYS, worldView);
        GX::GXBegin(GX_QUADS, 0, 4 * cpHolder->pointCount);
       
        float height = 0.0f;
        Quad2D quad;
        Vec2 itpt2D;
         KMP::CKPT *curCP = cpHolder->pointArray[0]->raw;
        for (int i = cpHolder->pointCount -1 ; i >= 0; i--){
            KMP::CKPT *nextCP = curCP;
            curCP = cpHolder->pointArray[i]->raw;
            quad.PointA = curCP->leftPoint;
            quad.PointB = nextCP->leftPoint;
            quad.PointC = nextCP->rightPoint;
            quad.PointD = curCP->rightPoint;
            for (int j = 0; j < itemPointHolder->pointCount; j++){
                KMP::ITPT *curITPT = itemPointHolder->pointArray[j]->raw;

                itpt2D.x = curITPT->position.x;
                itpt2D.z = curITPT->position.z;
                if (quad.IsInConvexQuad(&itpt2D)) 
                {
                    height = curITPT->position.y;
                    break;
                }
            }

            u32 colour = 0x0000ff70;
            if (curCP->type == 0) colour = 0xFF80FF70;
            else if (curCP->type != 0xFF) colour = 0xFF00FF70;

            GX::GX_Position3f32(curCP->leftPoint.x, height + 1500.0f, curCP->leftPoint.z); //these are really zs and not y
            GX::GX_Color1u32(colour);
            GX::GX_Position3f32(curCP->rightPoint.x, height + 1500.0f, curCP->rightPoint.z);
            GX::GX_Color1u32(colour);
            GX::GX_Position3f32(curCP->rightPoint.x, height - 300.0f, curCP->rightPoint.z);
            GX::GX_Color1u32(colour); 
            GX::GX_Position3f32(curCP->leftPoint.x, height - 300.0f, curCP->leftPoint.z);
            GX::GX_Color1u32(colour);
        }
        GXEnd();
    }
    else if(params->settings.scrollSetting[SCROLLER_INFO] == SCROLLER_INFO_ITPT){
        
        u8 id = raceData->main.scenarios->settings.hudPlayerIds[0];
        u32 myITPT = itemHolder->players[id].itemPoint.ITPT;
        for (int i = 0; i < itemPointHolder->pointCount; i++){
            KMP::ITPT *curITPT = itemPointHolder->pointArray[i]->raw;
            Vec3 *position = &curITPT->position;
            u32 colour = 0x00FF0030;
            if (myITPT == i) colour = 0xFF00FF30;
            DrawLineCylinderGX(worldView, position->x, position->y, position->z, curITPT->width * 1.5f , 400.0f, 30, 20, colour);
        }
    }
    else if(params->settings.scrollSetting[SCROLLER_INFO] == SCROLLER_INFO_AREA){
        
        KMP::AreaSectionHolder *areaHolder = control->area;
        for (int i = 0; i < areaHolder->pointCount; i++){
            KMP::AreaPoint *curAREAPoint = areaHolder->pointArray[i];
            KMP::AREA *curAREA = curAREAPoint->rawData;
            Vec3 *position = &curAREA->position;
            u32 colour = 0xFF000070;
            if(curAREA->type == 0){
                float x1 = position->x + curAREAPoint->halfWidth;
                float x2 = position->x - curAREAPoint->halfWidth;
                float y1 = position->y + curAREAPoint->height;
                float y2 = position->y;
                float z1 = position->z + curAREAPoint->halfLength;
                float z2 = position->z - curAREAPoint->halfLength;

                SetupGX(GX_ALWAYS, worldView);
                DrawRectangleGX(x1, x1, y1, y2, z1, z2, colour);
                DrawRectangleGX(x2, x2, y1, y2, z1, z2, colour);
                DrawRectangleGX(x1, x2, y1, y2, z1, z1, colour);
                DrawRectangleGX(x1, x2, y1, y2, z2, z2, colour);
                DrawRectangleGX(x1, x2, y1, y1, z1, z2, colour);
            }
            else{
                DrawLineCylinderGX(worldView, position->x, position->y, position->z, curAREAPoint->radius, curAREAPoint->height, 30, 30, colour);
            }
        }
    }
}
static RaceFrameHook DisplayCheckPointsHook(DisplayCheckPoints);