#pragma once
#include <Kamek/kamek.hpp>
#include <UI/menudata.hpp>
#include <UI/Screen/base.hpp>
#include <UI/Screen/ParentScreenBases.hpp>
#include <UI/Screen/SpecificScreens/SharedStructs.hpp>
#include <System/system.hpp>



typedef enum GhostManagerScreenState{
    IDLE,
    SAVED_GHOST_RACE_FROM_MENU = 0x3,
    SAVED_GHOST_REPLAY = 0x6,
    STAFF_GHOST_RACE_FROM_MENU = 0x9,
    STAFF_GHOST_REPLAY = 0xc
};

enum GhostGroupType{
    GROUP_SAVED,
    GROUP_DOWNLOADED,
    GROUP_EASY_STAFF,
    GROUP_EXPERT_STAFF,
    GROUP_GHOST_RACE,
    GROUP_COMPETITION,
    GROUP_NONE = 0xFF
};

class GhostManagerScreen;
struct GhostListEntry{
    static int Compare(const void *entry, const void *entry2); //ingame function 805e2430 obviously params aren't void
    GhostHeader *header;
    u32 licenseId;
    u32 index;
    bool isNew;
    u8 unknown_0xD[3];
}; //total size 0x10

class GhostList{
public:
    void Init(CourseId id); //0x805e2200
    void InitMenudata98Params(u32 entry); //805e2588
    GhostListEntry entries[38];
    u32 count;
    GhostManagerScreen *ghostManagerScreen;
}; //total size 0x268

class GhostGroup{
public:
    GhostGroup(EGG::ExpHeap *heap, u32 type); //0x8051ce94 vtable 808b2d38
    virtual ~GhostGroup(); //8051cff8
    GhostHeader *GetGhostHeader(u16 id);
    u16 count;
    u8 unknown_0x6[2];
    GhostGroupType ghostType;  //saved 0, downloaded 1, easy 2, expert 3, ghost race 4, competition 5
    GhostHeader *files[32]; //0xC
    u32 unknown;
}; //total size 0x14

class GhostManagerScreen : public Screen {
public:
    GhostManagerScreen(); //805e0c38 vtable 0x808b9258
    virtual ~GhostManagerScreen(); //0x805e0cb8
    virtual void OnInit(); //805e0d84
    virtual void OnLoad(); //805e0ffc
    virtual void OnUnload(); //805e1100
    virtual void OnActivate(); //805e10f8
    virtual void OnDefocus(); //805e10fc
    virtual void AfterChildUpdate(); //805e1104
    virtual void func_0x58(); //805e1108
    virtual int GetStaticInstanceAddr(); //805e2720

    void RequestGhost(u8 r4); //0x805e1998
    void SetupGhostRace(u32 r4, u32 r5, u32 r6);
    void SetupGhostReplay(u32 r4);
    void LoadGhost(u32 r4); //stores something in menudata90 which ultimately leads to the nand ghost getting loaded
    void UpdateStateToReplay();
    InputThing inputThing; //0x44
    GhostGroup *savedGhostGroups[4]; //0x54
    GhostGroup *downloadedGhostsGroup; //0x64
    GhostGroup *easyStaffGhostsGroup; //0x68
    GhostGroup *expertStaffGhostsGroup; //0x6C
    GhostGroup *unknown_group; //0x70
    bool isNew[0x20]; //for each track 
    GhostManagerScreenState state; //0x94
    u32 unknown_0x98;
    GhostManagerScreenState nextState; //0x9C
    GhostList list; //0xA0
    RKG *rkgPointer;
    RKG rkg2;
};//total size 0x2b0c

extern "C" {
    CourseId GetCourseIdBySlot(u32 slot); //just the nbr of the track
    void LoadStaffGhost(u32 ghostType, u32 courseId, RKG *rkgBuffer);
    int CourseIdToGhostIndex(CourseId id);
}