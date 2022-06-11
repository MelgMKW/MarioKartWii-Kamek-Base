#include <Kamek/kamek.hpp>
#include <System/system.hpp>
#include <Race/racedata.hpp>
#include <Race/kmp.hpp>
#include <Race/playerholder.hpp>
#include <Race/Item/itemholder.hpp>
#include <UI/Screen/base.hpp>
#include <UI/Screen/ParentScreenBases.hpp>
#include <UI/Screen/SpecificScreens/GhostSelect.hpp>
#include <CustomSettings.hpp>

Screen *CreateSettingsScreen(){
    if (menuData->curScene->menuId == OPTIONS_MENU){
        return new(CustomSettingsPanel)(3, 1);
    }
    else{
        return new(VSSettingScreen);
    }
}
kmCall(0x80623e1c, &CreateSettingsScreen);
kmWrite32(0x80623e10,0x60000000); //nop the original new


void* LoadRKG(char *path, void *buffer, EGG::Heap *heap, u32 allocDirection, u32 offset, u32 r8, u32 *r9){
    char *occurrence = strstr(path, "ghost");
    s32 fd = -1;
    if(occurrence != NULL){
        u32 ghostType;
        u32 trackId;
        sscanf(occurrence,"ghost%*d/ghost%d_comp_%d.rkg", &ghostType, &trackId);
        if(ghostType == 1) trackId += 0x20;
        char nandPath[0x30];
        snprintf(nandPath, 0x30, "/Ghosts/%02d.rkg", trackId);
        fd = ISFSOpen(nandPath, 1);
        if (fd >= 0){
            ISFSRead(fd, buffer, 0x2800);
            ISFSClose(fd);
        }
    }
    if (fd < 0 ) EGG::LoadToMainRam(path, buffer, heap, allocDirection, offset, r8, r9);
    return buffer;
}

void LoadCustomStaffGhost (SystemManager *systemManager, char *ghostPath, void *heap, u32 allocDirection, u32 r7, u32 *buffer){
    Scene *scene =  menuData->curScene;
    GhostSelectScreen *screen = (GhostSelectScreen*) scene->screens[SELECT_GHOST2];
    GhostList *list = screen->ghostList;
    u32 page = screen->page;
    GhostFile *ghostChosen = list->entries[page].file;

    GhostManagerScreen *ghostManager = (GhostManagerScreen*) scene->screens[GHOST_LIST_MANAGER];
    GhostFile *easyStaffGhostsFiles = ghostManager->easyStaffGhostsGroup->files[0];
    GhostFile *expertStaffGhostsFiles = ghostManager->expertStaffGhostsGroup->files[0];

    u32 arrayPosition = ghostChosen - easyStaffGhostsFiles;
    u32 ghostType = 0x1;
    if (arrayPosition > 31){
        arrayPosition = ghostChosen - expertStaffGhostsFiles;
        ghostType = 0x2;
    }
    char path[0x80];
    snprintf(path, 0x80, "/Race/TimeAttack/ghost%1d/ghost%1d_comp_%02d.rkg", ghostType, ghostType, arrayPosition);
    systemManager->RipFromDiscAsync(path, 0, 1, 0, buffer);
    return;
}
kmCall(0x8000b5ac, &LoadRKG);
kmCall(0x8054284c, &LoadCustomStaffGhost);
kmWrite32(0x805E237C, 0x38600002); //easy and experts are displayed no matter what
kmWrite32(0x805E2B0C, 0x38000000); //remove nintendo "flag"

u16 controllerOnStrap = 0x112;
asm int ControllerOnStrapPress(){
    ASM(
    mr r3, r31;
    cmpwi r3, 0;
    beq+ end;

    li r6, 0x3;
    addi r5, sp, 0x10;
    loop:;
    mulli r4, r6, 0xC;
    lhzx r4, r5, r4;
    cmpwi r4, 0x0;
    addi r4, r6, 0x1;
    slwi r4, r4, 0x8;
    addi r4, r4, 0x24;
    bne- store;

    subic. r6, r6, 1;
    bge+ loop;

    slwi r5, r29, 0x8;
    lbz r6, 0x9F (sp);
    addi r4, r5, 0x11;
    cmpwi r6, 0x2;     //wheel
    beq- store;
    addi r4, r5, 0x12;
    cmpwi r6, 0x5;     //nunchuck
    beq+ store;
    addi r4, r5, 0x13;

    store:;
    lis r5, controllerOnStrap@ha;
    sth r4, controllerOnStrap@l (r5);
    end:;
    )
}
kmCall(0x800079b0, &ControllerOnStrapPress);

char bootParams[16] = "-s132 -l0 -p274";
MenuType BootIntoCupSelect(){
    RacedataScenario *scenario =  &raceData->main.scenarios[1];
    scenario->settings.gamemode = MODE_TIME_TRIAL;
    scenario->settings.engineClass = CC_150;

    for (u8 i = 1; i<0xC; i++){
        scenario->players[i].playerType = PLAYER_NONE;
    }

    scenario->players[0].characterId = FUNKY_KONG;
    scenario->players[0].vehicleId = BOWSER_BIKE;

    MenuData98 *menuData98 = menuData->menudata98;
    menuData98->characters[0] = FUNKY_KONG;
    menuData98->vehicles[0] = BOWSER_BIKE;

    snprintf(bootParams, 16, "-s132 -l0 -p%d", controllerOnStrap);
    systemManager->ndevArg = bootParams;
    SetupMenuLoad();

    return SINGLE_PLAYER_TT_CHANGE_COURSE;
}
kmCall(0x80634f20, &BootIntoCupSelect);

void SetUpCorrectController(RealControllerHolder *realControllerHolder, void* inputDataSub, u32 r5){  //r4 usually uses inputdata3 which is slot and controller independant
    u32 controllerInfo = menuData->sub.controllerInfos[0].controllerSlotAndType;  //technically hooking into a loop 
    ControllerType controllerType = ControllerType(controllerInfo & 0xFF);
    u32 slot =((controllerInfo & 0xFF00) >>0x8) - 1; //to make it 0-indexed
    InputData *input = inputData;
    if (controllerType == GCN) inputDataSub = (void*) &input->gcnControllers[slot];
    else inputDataSub = (void*) &input->wiiControllers[slot];
    StoreCorrectInputDataSub(realControllerHolder, inputDataSub, r5);
    return;
}
kmCall(0x805243f4, &SetUpCorrectController);

void* NANDSZSLoader(char *path, void* buffer, EGG::Heap *heap, u32 allocDirection, u32 offset, u32 r8, u32 *sizeTmp){
    s32 fd = -1;
    char *occurrence = strstr(path, "Race/Course/");
    if (occurrence != NULL){
        char trackName[0x30];
        sscanf(occurrence, "Race/Course/%s", &trackName);
        char nandPath[0x50];
        snprintf(nandPath, 0x50, "/Course/%s", trackName);
        fd = ISFSOpen(nandPath, 1);
        if (fd >= 0){
            FileStats fileStats __attribute((aligned (0x20)));
            ISFSGetFileStats(fd, &fileStats);
            u32 fileSize = fileStats.fileSize;
            buffer = EGG::Heap::alloc(fileSize, 0x20, heap);
            ISFSRead(fd, buffer, fileSize);
            ISFSClose(fd);
            *sizeTmp = fileSize;
            return buffer;
        }
    }
    return EGG::LoadToMainRam(path, buffer, heap, allocDirection, offset, r8, sizeTmp);
}
kmCall(0x80518e94, &NANDSZSLoader);
kmWrite32(0x805491B4, 0x41820018); //ghost always saves
kmWrite32(0x80857aa0, 0x60000000); //disable ghost cannot be saved message
