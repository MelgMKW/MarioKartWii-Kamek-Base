#pragma once
#include <Kamek/kamek.hpp>
#include <nw4r/snd/snd.hpp>
#include <System/camera.hpp>
#include <Input/inputdata.hpp>
#include <Sound/musichandler.hpp>
#include <Race/Item/itemobjproperties.hpp>
#include <UI/Ctrl/Ctrl.hpp>

bool CheckButtonPressed(u8 playerHudId, UniversalButtons button);
extern "C"{
    void SetMatIcon(UnkType *r3, char *materialName, char *iconPaneName);
    int PlayBRSARSound(int r3, u32 soundId, u32 r5);
    void ChangeMusic(MusicHandler *musichandler, RaceState raceState);
    u32 LoadFromU8(UnkType *archive, u8 src, char *fileName, UnkType *r6);
    void CopyItemOBJPropertiesFromRelToTable(int *itemTable, ItemOBJProperties *itemObjProperties);
}

class CtrlRaceSpeedo : public CtrlRaceBase {
public:
    virtual void Init();
    virtual void OnUpdate();
    void Load(char* variant, u8 id);
};

class CtrlRaceDebug : public CtrlRaceBase {
public:
    virtual void Init();
    virtual void OnUpdate();
    void Load();
};

enum RadioSettings {
    RADIO_FASTMENUS = 0x0,
    RADIO_DEBUG = 0x1,
    RADIO_SPEEDUP = 0x2,
};

enum ScrollerSettings {
    SCROLLER_BOOT = 0x0,
    SCROLLER_INFO = 0x1
};

enum RadioFASTMENUS {
    RADIO_FASTMENUS_DISABLED = 0x0,
    RADIO_FASTMENUS_ENABLED = 0x1,
};

enum RadioDEBUG {
    RADIO_DEBUG_DISABLED = 0x0,
    RADIO_DEBUG_LIGHT = 0x1,
    RADIO_DEBUG_FULL = 0x2
};

enum RadioSPEEDUP {
    RADIO_SPEEDUP_DISABLED = 0x0,
    RADIO_SPEEDUP_ENABLED = 0x1
};

enum ScrollerBOOT {
    SCROLLER_BOOT_DEFAULT = 0x0,
    SCROLLER_BOOT_TTCUP = 0x1,
    SCROLLER_BOOT_1PMENU = 0x2,
    SCROLLER_BOOT_ONLINE = 0x3
};

enum ScrollerINFO {
    SCROLLER_INFO_NONE = 0x0,
    SCROLLER_INFO_CHECKPOINT = 0x1,
    SCROLLER_INFO_ITPT = 0x2,
    SCROLLER_INFO_AREA = 0x3
};

