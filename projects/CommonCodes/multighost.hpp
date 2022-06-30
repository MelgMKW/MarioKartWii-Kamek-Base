#pragma once
#include <Kamek/kamek.hpp>
#include <UI/Screen/SpecificScreens/GhostManager.hpp>
#include <UI/Screen/SpecificScreens/GhostSelect.hpp>
#include <ToggleButton.hpp>
#include <Race/racedata.hpp>
#include <System/system.hpp>

extern u32 groupTypeSaveOffset[6];

enum CustomGhostGroupType{
    
    CUSTOMGROUP_SAVED,
    CUSTOMGROUP_DOWNLOADED,
    CUSTOMGROUP_EASY_STAFF,
    CUSTOMGROUP_EXPERT_STAFF,
    CUSTOMGROUP_GHOST_RACE,
    CUSTOMGROUP_COMPETITION,
    CUSTOMGROUP_CUSTOM,
    CUSTOMGROUP_NONE = 0xFF
};

class CustomGhostGroup{
public:
    CustomGhostGroup(){
        files = NULL;
    };
    ~CustomGhostGroup(){};
    void Init();
    GhostHeader *GetGhostHeader(u16 id);
    u16 count;
    u8 unknown_0x2[2];
    CustomGhostGroupType type;  //saved 0, downloaded 1, easy 2, expert 3, ghost race 4, competition 5 0x4
    GhostHeader *files; //0x8
    u32 unknown;
}; //0x10

struct GhostHolder{
    u32 licenseId;
    CustomGhostGroupType groupType;
    u32 index; //used
};

class MultiGhostHolder{
public:
    MultiGhostHolder(){};
    void Init();
    void ResetHolders();
    bool AddGhost(GhostListEntry *entry, GhostManagerScreen *screen, bool isMain);
    void RemoveGhost(GhostListEntry *entry,  GhostManagerScreen *screen);
    static bool LoadGhost(RKG *rkg, GhostHolder *holder);
    static void CopyRKGAndInitParams(RKG *src, u8 id, bool isGhostRace);
    CustomGhostGroup group;
    GhostHolder mainGhost; //from the watch replay;
    GhostHolder ghostHolders[3];
    u32 lastUsedSlot;
    static MultiGhostHolder *sInstance;
};

class ExtendedGhostSelectScreen : public GhostSelectScreen{
public:
    ExtendedGhostSelectScreen();
    virtual void OnInit();
    virtual void OnLoad();
    void HandleWatchReplayPress(PushButton *button, u32 handlerId);
    void HandleOnSelectGhostChange(ToggleButton *button);
    void HandleRightArrowPress(SheetSelectControl *control, u32 handlerId);
    void HandleLeftArrowPress(SheetSelectControl *control, u32 handlerId);
    void Reset();
    ToggleButton selectGhostButton;
    ToggleButtonChangeHandler *onSelectGhostChangeHandler;
    s8 selectedGhostsPages[3];
    u8 selectedGhostsCount;
};

