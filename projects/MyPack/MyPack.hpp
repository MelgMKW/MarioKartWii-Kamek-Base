#pragma once
#include <Kamek/kamek.hpp>
#include <System/system.hpp>
#include <Race/lakitucontroller.hpp>
#include <Sound/musichandler.hpp>

class ItemHolderPlayer;

bool CheckButtonPressed(RealControllerHolder *controllerHolder, ControllerType controllerType, bool onlyCheckNewButtons, u32 button);

extern "C"{
    void StoreFlapIntoSave(UnkType *saveDataManagerLicense, UnkType *timer, u32 r5, CourseId id);
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

extern float screenTransitionDelay;