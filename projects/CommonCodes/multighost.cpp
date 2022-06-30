#include <Kamek/kamek.hpp>
#include <multighost.hpp>
#include <project.hpp>
#include <System/saveDataManager.hpp>

#if MULTIGHOST == 1
int IncreaseRacedataSize(){
    return 0xC3F0;
}
kmCall(0x8052fe78, &IncreaseRacedataSize); //make racedata bigger to have 2 more rkgs in main
kmWrite32(0x80531f44, 0x4800001c); //make it so the game will only use the first rkg buffer for normal ghost usage

MultiGhostHolder* MultiGhostHolder::sInstance = NULL;

void CreateCustomGhostGroup(){
    RKSystem *system = &rkSystem;
    MultiGhostHolder::sInstance = new (system->EGGSystem) MultiGhostHolder;
}
static BootHook CreateGhosts(CreateCustomGhostGroup);

void CustomGhostGroup::Init(){
    if(this->files != NULL) delete[] this->files;
    this->count = 0;
    this->files = NULL;
    this->type =  CUSTOMGROUP_CUSTOM;
    u32 fileCount = 0;
    s32 fd = ISFSReadDir("/Ghosts", 0, &fileCount);
    if (fd >= 0){
        this->count = fileCount;
        RKSystem *system = &rkSystem;
        this->files = new (system->EGGSystem)GhostHeader[fileCount];
        u32 index = 0;
        char path[0x10];
        RKG *buffer = (RKG*) EGG::Heap::alloc(0x2800, 0x20, 0);
        for (int i = 0; i < fileCount; i++){
            snprintf(path, 0x10, "/Ghosts/%02d.rkg", i);
            fd = ISFSOpen(path, 1);
            if(fd >= 0){
                FileStats fileStats __attribute((aligned (0x20)));
                ISFSGetFileStats(fd, &fileStats);
                ISFSRead(fd, buffer, fileStats.fileSize);
                this->files[i].Init(buffer);
                this->files[i].type = (GhostType) 0x27;
                ISFSClose(fd);
            }
        }
        delete(buffer);
    }
}

void MultiGhostHolder::Init(){
    this->group.Init();
    this->ResetHolders();
}

void MultiGhostHolder::ResetHolders(){
    this->lastUsedSlot = 0;
    GhostHolder *holder = &this->mainGhost;
    holder->licenseId = 0xFF;
    holder->groupType = CUSTOMGROUP_NONE;
    holder->index = 0xFF;
    for(int i = 0; i < 3; i++){
        holder = &this->ghostHolders[i];
        holder->licenseId = 0xFF;
        holder->groupType = CUSTOMGROUP_NONE;
        holder->index = 0xFF;
    }
}

bool MultiGhostHolder::AddGhost(GhostListEntry *entry, GhostManagerScreen *screen, bool isMain){
    bool exists = false;
    bool isCustom = false;
    GhostHeader *header = entry->header;

    CustomGhostGroup *group = &this->group;

    GhostHolder *ghostHolder = &this->ghostHolders[this->lastUsedSlot];
    if(isMain) ghostHolder = &this->mainGhost;
    for(int i = 0; i<group->count; i++){
        GhostHeader *customHeader = &group->files[i];
        if(customHeader == header){    
            ghostHolder->licenseId = 0;
            ghostHolder->groupType = CUSTOMGROUP_CUSTOM;
            ghostHolder->index = i;
            exists = true;
            isCustom = true;
            break;
        }
    }
    
    if(!isCustom){
        GhostType type = header->type;
        if(type == BEST_TIME){
        ghostHolder->licenseId = entry->licenseId;
        ghostHolder->groupType = CUSTOMGROUP_SAVED;
        ghostHolder->index = 0;
        exists = true;
        }
        else if (type <= WORLD_RECORD && type <= FRIEND_GHOST){
            GhostGroup *downloaded = screen->downloadedGhostsGroup;
            for(int i = 0; i < downloaded->count; i++){
                GhostHeader *comparedheader = downloaded->GetGhostHeader(i);
                if ((comparedheader != NULL) && (comparedheader->type == type) && (comparedheader->courseId ==header->courseId)){
                    ghostHolder->licenseId = 0;
                    ghostHolder->groupType = CUSTOMGROUP_DOWNLOADED;
                    ghostHolder->index = i;
                    exists = true;
                    break;
                }
            }
        }
        else if (type == EXPERT_STAFF_GHOST || type == EASY_STAFF_GHOST){
            ghostHolder->licenseId = 0;
            ghostHolder->groupType = type == EXPERT_STAFF_GHOST ? CUSTOMGROUP_EXPERT_STAFF : CUSTOMGROUP_EASY_STAFF;
            ghostHolder->index = 0;
            exists = true;
        }           
    }
    if(exists && !isMain) this->lastUsedSlot = (this->lastUsedSlot + 1) %3;
    return exists;
}

void MultiGhostHolder::RemoveGhost(GhostListEntry *entry, GhostManagerScreen *screen){
    bool isCustom = false;
    GhostHeader *header = entry->header;
    GhostType type = header->type;

    u32 licenseId = 0xFF;
    CustomGhostGroupType groupType = CUSTOMGROUP_NONE;
    u32 index = 0xFF;

    CustomGhostGroup *group = &this->group;
    for(int i = 0; i<group->count; i++){
        GhostHeader *customHeader = &group->files[i];
        if(customHeader == header){
            licenseId = 0;
            groupType = CUSTOMGROUP_CUSTOM;
            index = i;
            isCustom = true;
            break;
        }
    }

    if(!isCustom){
        if(type == BEST_TIME){
            licenseId = entry->licenseId;
            groupType = this->ghostHolders[lastUsedSlot].groupType = CUSTOMGROUP_SAVED;
            index = 0;
        }
        else if (type <= WORLD_RECORD && type <= FRIEND_GHOST){
            GhostGroup *downloaded = screen->downloadedGhostsGroup;
            for(int i = 0; i < downloaded->count; i++){
                GhostHeader *comparedheader = downloaded->GetGhostHeader(i);
                if ((comparedheader != NULL) && (comparedheader->type == type) && (comparedheader->courseId ==header->courseId)){
                    licenseId = 0;
                    groupType = CUSTOMGROUP_DOWNLOADED;
                    index = i;
                    break;
                }
            }
        }
        else if (type == EXPERT_STAFF_GHOST || type == EASY_STAFF_GHOST){
            licenseId = 0;
            groupType = type == EXPERT_STAFF_GHOST ? CUSTOMGROUP_EXPERT_STAFF : CUSTOMGROUP_EASY_STAFF;
            index = 0;

        }           
    }
    for(int i = 0; i < 3; i++){
        GhostHolder *holder = &this->ghostHolders[i];
        if(holder->licenseId == licenseId && holder->groupType == groupType && holder->index == index){
            this->lastUsedSlot = i;
            holder->licenseId = 0xFF;
            holder->groupType = CUSTOMGROUP_NONE;
            holder->index = 0xFF;
        }
    }
}

bool MultiGhostHolder::LoadGhost(RKG *rkg, GhostHolder *holder){
    rkg->ClearBuffer();
    CustomGhostGroupType type = holder->groupType;
    CourseId courseId = menuData->menudata98->courseId;
    char path[0x80];
    switch(holder->groupType){
        case CUSTOMGROUP_SAVED:
        case CUSTOMGROUP_DOWNLOADED:
        {
            u32 index = holder->index;
            if(type == CUSTOMGROUP_SAVED){
                for(int i = 0; i < 32; i++){
                    CourseId curId = GetCourseIdBySlot(i);
                    if(curId == courseId){
                        index =i;
                        break;
                    }
                }
            }
            u32 offset = (holder->licenseId * 0x42 + index + groupTypeSaveOffset[type]) * 0x2800 + 0x28000;
            nandSaveManager->Read(rkg, 0x2800, offset, true);
            break;
        }
        case CUSTOMGROUP_EXPERT_STAFF:
        case CUSTOMGROUP_EASY_STAFF:
        {
            u32 ghostIndex = CourseIdToGhostIndex(courseId);
            u32 folderId = 1;
            if(type == CUSTOMGROUP_EXPERT_STAFF) folderId = 2;
            snprintf(path, 0x80, "/Race/TimeAttack/ghost%1d/ghost%1d_comp_%02d.rkg", folderId, folderId, ghostIndex);
            EGG::LoadToMainRam(path, rkg, 0, 1, 0, 0, 0);
            break;
        }
        case CUSTOMGROUP_CUSTOM:
        {
            snprintf(path, 0x80, "/Ghosts/%02d.rkg", holder->index);
            s32 fd = ISFSOpen(path, 1);
            if (fd >= 0){
                FileStats fileStats __attribute((aligned (0x20)));
                ISFSGetFileStats(fd, &fileStats);
                ISFSRead(fd, rkg, fileStats.fileSize);
                ISFSClose(fd);
            }
            break;
        }
    }
    if(strcmp(rkg->header.magic, "RKGD")) return true;
    return false;
}

void MultiGhostHolder::CopyRKGAndInitParams(RKG *src, u8 id, bool isGhostRace){
    RacedataMain *main = &raceData->main;
    RKG *dest = &main->ghosts[id];
    if(src->header.compressedAndType & 0x800) src->Decompress(dest); //0x2800
    else memcpy(dest, src, 0x2800);
    main->scenarios[1].players[id + isGhostRace].playerType = PLAYER_GHOST;
    GhostHeader header(dest);
    menuData->menudata98->playerMiis.Add(id + isGhostRace, &header.miiData);
}


void InsertCustomGroupToList(GhostList *list, CourseId id){
    list->Init(id);
    CustomGhostGroup *group = &MultiGhostHolder::sInstance->group;
    u32 totalGhostsCount = list->count + group->count;
    if (totalGhostsCount > 38) totalGhostsCount = 38;
    u32 iterator = list->count;
    for(int i = 0; i < group->count; i++){
        GhostHeader *header = &group->files[i];
        if(header->courseId == id){
            list->entries[iterator].header = header;
            iterator++;
        } 
    }
    list->count = iterator;
    qsort(list, list->count, sizeof(GhostListEntry), &GhostListEntry::Compare);
};
kmCall(0x806394f0, &InsertCustomGroupToList);

ExtendedGhostSelectScreen *CreateExpandedSelectGhostScreen(){
    MultiGhostHolder::sInstance->group.Init(); //only init when this screen gets built so that the group stays the same in races
    return new(ExtendedGhostSelectScreen);
};
kmCall(0x80623dec, &CreateExpandedSelectGhostScreen);
kmWrite32(0x80623de0, 0x60000000); //nop the new
kmWrite32(0x80639958, 0x3880000A); //Add one control

ExtendedGhostSelectScreen::ExtendedGhostSelectScreen(){
    this->onWatchReplayClickHandler.ptmf = static_cast<void(Screen::*)(PushButton*, u32)>(&ExtendedGhostSelectScreen::HandleWatchReplayPress);
    this->onRightArrowPressHandler.ptmf = static_cast<void(Screen::*)(SheetSelectControl*, u32)>(&ExtendedGhostSelectScreen::HandleRightArrowPress);
    this->onLeftArrowPressHandler.ptmf = static_cast<void(Screen::*)(SheetSelectControl*, u32)>(&ExtendedGhostSelectScreen::HandleLeftArrowPress);
    //this->onBackPress.ptmf = static_cast<void(Screen::*)(u32)>(&ExtendedGhostSelectScreen::HandleBackPress);
}

void ExtendedGhostSelectScreen::OnInit(){
    GhostSelectScreen::OnInit();
    this->backButton.isHidden = true;
    this->backButton.actionHolder.inacessible = true;
    ToggleButton *button = &this->selectGhostButton;
    this->AddControl(9, button, 0);
    button->buttonId = 9;
    button->InitLayout("control", "SelGhostButton", "SelectGhost", 1, 0, false);
    this->onSelectGhostChangeHandler = new(ToggleButtonChangeHandler);
    this->onSelectGhostChangeHandler->onChangeHandler.subject = this;
    this->onSelectGhostChangeHandler->onChangeHandler.ptmf = static_cast<void (Screen::*)(ToggleButton*)>(&ExtendedGhostSelectScreen::HandleOnSelectGhostChange);
    button->SetOnChangeHandler(this->onSelectGhostChangeHandler);
    this->Reset();
}

void ExtendedGhostSelectScreen::HandleWatchReplayPress(PushButton *button, u32 handlerId){
    GhostSelectScreen::HandleWatchReplayPress(button, handlerId);
    GhostListEntry *entry = &this->ghostList->entries[this->page];
    GhostManagerScreen *ghostManager = (GhostManagerScreen*) menuData->curScene->screens[GHOST_LIST_MANAGER];
    MultiGhostHolder::sInstance->AddGhost(entry, ghostManager, true);
}

void ExtendedGhostSelectScreen::HandleOnSelectGhostChange(ToggleButton *button){
    MultiGhostHolder *holder = MultiGhostHolder::sInstance;
    GhostListEntry *entry = &this->ghostList->entries[this->page];
    GhostManagerScreen *ghostManager = (GhostManagerScreen*) menuData->curScene->screens[GHOST_LIST_MANAGER];
    u32 index = holder->lastUsedSlot;
    if(button->state == true){
        if(holder->AddGhost(entry, ghostManager, false))
        {
            this->selectedGhostsPages[index] = this->page;
            this->selectedGhostsCount = (this->selectedGhostsCount + 1) > 3 ? 3 : (this->selectedGhostsCount + 1);
        } 
        else button->ToggleState(false);
        
    }
    else{
        holder->RemoveGhost(entry, ghostManager);
        this->selectedGhostsPages[index] = -1;
        this->selectedGhostsCount -= 1;
    }
    ScreenText text;
    text.intToPass[0] = this->selectedGhostsCount;
    this->ctrlMenuPageTitleText.SetMsgId(0x2402, &text);
}

void ExtendedGhostSelectScreen::HandleRightArrowPress(SheetSelectControl *control, u32 handlerId){
    GhostSelectScreen::HandleRightArrowPress(control, handlerId);
    ToggleButton *button = &this->selectGhostButton;
    if (this->page == this->selectedGhostsPages[0] || this->page == this->selectedGhostsPages[1] || this->page == this->selectedGhostsPages[2]){
        if(button->state == false){
            button->ToggleState(true);
        }
    }
    else if(button->state == true) button->ToggleState(false);
}

void ExtendedGhostSelectScreen::HandleLeftArrowPress(SheetSelectControl *control, u32 handlerId){
    GhostSelectScreen::HandleLeftArrowPress(control, handlerId);
    ToggleButton *button = &this->selectGhostButton;
    if (this->page == this->selectedGhostsPages[0] || this->page == this->selectedGhostsPages[1] || this->page == this->selectedGhostsPages[2]){
        if(button->state == false){
            button->ToggleState(true);
        }
    }
    else if(button->state == true) button->ToggleState(false);
}

void ExtendedGhostSelectScreen::OnLoad(){
    GhostSelectScreen::OnLoad();
    this->ctrlMenuPageTitleText.SetMsgId(0xD4F, 0);
    this->Reset();
}

void ExtendedGhostSelectScreen::Reset(){
    this->selectedGhostsPages[0] = -1;
    this->selectedGhostsPages[1] = -1;
    this->selectedGhostsPages[2] = -1;
    this->selectedGhostsCount = 0;
    this->selectGhostButton.ToggleState(false);
    MultiGhostHolder::sInstance->ResetHolders();
}

void LoadCorrectMainGhost(GhostManagerScreen *ghostManager, u8 r4){
    GhostType type = menuData->menudata98->ghostType;
    if(type == (GhostType) 0x27){
        MultiGhostHolder *holder = MultiGhostHolder::sInstance;
        holder->LoadGhost(ghostManager->rkgPointer, &holder->mainGhost);
        ghostManager->state = (GhostManagerScreenState)(ghostManager->state + 0x6); //faking that it's a staff so it copies from the buffer and not savadatemanager
    }
    else ghostManager->RequestGhost(r4);
}
kmCall(0x805e158c, &LoadCorrectMainGhost);

void ExtendSetupGhostRace(GhostManagerScreen *ghostManager, u32 r4, u32 r5, u32 r6){
    ghostManager->SetupGhostRace(r4, r5, r6);

    MultiGhostHolder *holder = MultiGhostHolder::sInstance;
    RKG *rkg = ghostManager->rkgPointer;
    RacedataMain *main = &raceData->main;
    RacedataScenario *scenario = &main->scenarios[1];
    u8 position = 1;
    for(int i = 0; i < 2; i++){
        GhostHolder *ghostHolder = &holder->ghostHolders[i];
        if(ghostHolder->licenseId != 0xFF && ghostHolder->groupType != CUSTOMGROUP_NONE && ghostHolder->index != 0xFF){
            if(holder->LoadGhost(rkg, ghostHolder)){
                holder->CopyRKGAndInitParams(rkg, position, true);
                position++;
            }
        } 
    }
}
kmCall(0x805e13ac, &ExtendSetupGhostRace);
kmCall(0x805e13e4, &ExtendSetupGhostRace);
kmCall(0x805e141c, &ExtendSetupGhostRace);
kmCall(0x805e149c, &ExtendSetupGhostRace);
kmCall(0x805e14c8, &ExtendSetupGhostRace);
kmCall(0x805e14f4, &ExtendSetupGhostRace);

void ExtendSetupGhostReplay(GhostManagerScreen *ghostManager, u32 r4){
    ghostManager->SetupGhostReplay(r4);

    MultiGhostHolder *holder = MultiGhostHolder::sInstance;
    RKG *rkg = ghostManager->rkgPointer;
    RacedataMain *main = &raceData->main;
    RacedataScenario *scenario = &main->scenarios[1];
    u8 position = 1;
    for(int i = 0; i < 3; i++){
        GhostHolder *ghostHolder = &holder->ghostHolders[i];
        if(ghostHolder->licenseId != 0xFF && ghostHolder->groupType != CUSTOMGROUP_NONE && ghostHolder->index != 0xFF){
            if(holder->LoadGhost(rkg, ghostHolder)){
                holder->CopyRKGAndInitParams(rkg, position, false);
                position++;
            }
        } 
    }
}
kmCall(0x805e144c, &ExtendSetupGhostReplay);
kmCall(0x805e1518, &ExtendSetupGhostReplay);


void RacedataCheckCorrectRKG(u8 id){
    u8 offset = 0;
    if (raceData->main.scenarios[1].players[0].playerType != PLAYER_GHOST) offset = 1;
    raceData->main.ghosts[id - offset].CheckValidity();
}
kmWrite32(0x8052f5c0, 0x5763063E);
kmCall(0x8052f5c8, &RacedataCheckCorrectRKG);

void GhostHeaderGetCorrectRKG(GhostHeader *header, u8 id){
    u8 offset = 0;
    RacedataScenario *scenario = &raceData->main.scenarios[1];
    if (scenario->players[0].playerType != PLAYER_GHOST) offset = 1;
    scenario->players[id].localPlayerNum = id;
    scenario->settings.hudPlayerIds[id] = id;
    header->Init(&raceData->main.ghosts[id - offset]);
}
kmWrite32(0x8052f5dc, 0x5764063E);
kmCall(0x8052f5e4, &GhostHeaderGetCorrectRKG);

/*
void UseCorrectGhostControllerSlot(InputData *data, u8 id, RKGInputData *rkgdata, bool isAuto){
    if(raceData->main.scenarios[1].players[0].playerType != PLAYER_GHOST) id = id - 1;
    data->InitGhostController(id, rkgdata, isAuto);
}
kmCall(0x8052f610, &UseCorrectGhostControllerSlot);
*/

#endif