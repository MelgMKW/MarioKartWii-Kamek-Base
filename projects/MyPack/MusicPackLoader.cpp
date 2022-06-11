#include <Kamek/kamek.hpp>
#include <nw4r/snd/snd.hpp>
#include <Sound/musichandler.hpp>
#include <UI/Ctrl/Ctrl.hpp>
#include <UI/menudata.hpp>

u8 musicPackCount = 6;
s8 currentMusicPack = 0;
s8 currentMusicSlot = 33;

nw4r::snd::DVDSoundArchive::DVDFileStream *LoadCustomBRSTM(nw4r::snd::DVDSoundArchive *dvdSoundArchive, nw4r::snd::DVDSoundArchive::DVDFileStream *dvdFileStream, 
u32 size, char *path, u32 begin, u32 length){
    char fileName[0x50];
    sscanf(path, "/sound/strm/%s", &fileName);
    if ((fileName[0] != 'o') && currentMusicPack == 0) currentMusicPack = (nw4r::snd::DetailUtilCalcRandom() % musicPackCount) + 1;
    char customPath[0x80];
    snprintf(customPath, 0x80, "/sound/packs/%1d%s", currentMusicPack, fileName);
    s32 entryNum = DVDConvertPathToEntryNum(customPath);
    if(entryNum >= 0) path = customPath;
    return dvdSoundArchive->OpenExtStream(dvdFileStream, size, path, begin, length);
}
kmCall(0x8009e0e4, &LoadCustomBRSTM);



CourseId GetCustomMusicSlot(){
    if (currentMusicSlot == 33) return GetCourseMusicSlot();
    return trackToCourseIDTable[currentMusicSlot].courseID;
}
kmCall(0x80711fd8, &GetCustomMusicSlot);

void CustomCtrlMenuBottomMessage(CtrlMenuInstructionText *control){
    MenuType curMenu = menuData->curScene->menuId;
    if (curMenu == SINGLE_PLAYER_FROM_MENU || curMenu ==  SINGLE_PLAYER_TT_CHANGE_CHARA || curMenu == SINGLE_PLAYER_TT_CHANGE_COURSE || curMenu == SINGLE_PLAYER_VS_NEXT_RACE){
        RealControllerHolder *controllerHolder = &inputData->realControllerHolders[0];
        u16 curFrameInputs = controllerHolder->inputStates[0].buttonRaw;
        u16 prevFrameInputs = controllerHolder->inputStates[1].buttonRaw;

        s8 iterator = 0x0;
        if ((curFrameInputs & NUNCHUCK_DPAD_UP) && !(prevFrameInputs & NUNCHUCK_DPAD_UP)) iterator = 1;
        else if ((curFrameInputs & NUNCHUCK_DPAD_DOWN) && !(prevFrameInputs & NUNCHUCK_DPAD_DOWN)) iterator = -1; //-1 but % are only positive
        else if ((curFrameInputs & NUNCHUCK_DPAD_RIGHT) && !(prevFrameInputs & NUNCHUCK_DPAD_RIGHT)) iterator = 5;
        else if ((curFrameInputs & NUNCHUCK_DPAD_LEFT) && !(prevFrameInputs & NUNCHUCK_DPAD_LEFT)) iterator = -5;

        if(curFrameInputs & NUNCHUCK_Z){
            currentMusicPack = (iterator + currentMusicPack) % (musicPackCount + 1);
            if (currentMusicPack < 0) currentMusicPack += musicPackCount + 1 ;
        }
        else if(curFrameInputs & NUNCHUCK_C){
            currentMusicSlot = (iterator + currentMusicSlot) % (32 + 1); //32 tracks
            if (currentMusicSlot <= 0) currentMusicSlot += 32 + 1;
        }
        
        ScreenText text;
        u32 bmgId = 0x2178;
        if ((currentMusicPack != 0x0) && (currentMusicSlot == 33)){
            text.intToPass[0] = currentMusicPack;
            bmgId = 0x2179;
        }
        else if ((currentMusicPack == 0x0) && (currentMusicSlot != 33)){
            text.intToPass[0] = currentMusicSlot;
            bmgId = 0x217A;
        }        
        else if ((currentMusicPack != 0x0) && (currentMusicSlot != 33)){
            text.intToPass[0] = currentMusicPack;
            text.intToPass[1] = currentMusicSlot;
            bmgId = 0x217B;
        }
        control->SetMsgId(bmgId, &text);
    }
    return;
}
kmWritePointer(0x808D36B4, CustomCtrlMenuBottomMessage);