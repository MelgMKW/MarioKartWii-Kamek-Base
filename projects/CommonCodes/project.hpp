#pragma once
#include <Kamek/kamek.hpp>
#include <Input/inputdata.hpp>
#include <UI/menudata.hpp>

#ifdef __INTELLISENSE__
#define NANDSZSLOADER 1
#define GHOSTLOADER 1
#define BOOTMENUS 1
#define MULTIGHOST 1
#define WIIMMFI 1
#else
#endif

enum UniversalButtons{
  BUTTON_NONE             = 0,
  BUTTON_DPAD_LEFT        = 1 << 0,
  BUTTON_DPAD_RIGHT       = 1 << 1,
  BUTTON_DPAD_DOWN        = 1 << 2,
  BUTTON_DPAD_UP          = 1 << 3,
  BUTTON_A                = 1 << 4,
  BUTTON_B                = 1 << 5,
  BUTTON_X                = 1 << 6,
  BUTTON_Y                = 1 << 7,
  BUTTON_MINUS            = 1 << 8,
  BUTTON_PLUS             = 1 << 9,
  BUTTON_HOME             = 1 << 10,
  BUTTON_1                = 1 << 11,
  BUTTON_2                = 1 << 12,
  BUTTON_L                = 1 << 13,
  BUTTON_R                = 1 << 14,
  BUTTON_C                = 1 << 15,
  BUTTON_Z                = 1 << 16,
  BUTTON_ZL               = 1 << 17,
  C_STICK_UP_EIGHT        = 1 << 18,
  C_STICK_UPRIGHT_EIGHT   = 1 << 19,
  C_STICK_RIGHT_EIGHT     = 1 << 20,
  C_STICK_DOWNRIGHT_EIGHT = 1 << 21,
  C_STICK_DOWN_EIGHT      = 1 << 22,
  C_STICK_DOWNLEFT_EIGHT  = 1 << 23,
  C_STICK_LEFT_EIGHT      = 1 << 24,
  C_STICK_UPLEFT_EIGHT    = 1 << 25,
};

class Quad2D{
public:
    bool IsInConvexQuad(Vec2 *point);
    Vec2 PointA;
    Vec2 PointB;
    Vec2 PointC;
    Vec2 PointD;
};

class RaceLoadHook {
private:
    typedef void (Func)();
    Func *mFunc;
    RaceLoadHook * mNext;

    static RaceLoadHook * sHooks;

public:
    RaceLoadHook(Func * f) {
        mNext = sHooks;
        sHooks = this;
        mFunc = f;
    }

    static void exec() {
        for (RaceLoadHook * p = sHooks; p; p = p->mNext)
            p->mFunc();
    }
};

class RaceFrameHook {
private:
    typedef void (Func)();
    Func *mFunc;
    RaceFrameHook * mNext;

    static RaceFrameHook * sHooks;

public:
    RaceFrameHook(Func * f) {
        mNext = sHooks;
        sHooks = this;
        mFunc = f;
    }

    static void exec() {
        for (RaceFrameHook * p = sHooks; p; p = p->mNext)
            p->mFunc();
    }
};

class BootHook {
private:
    typedef void (Func)();
    Func *mFunc;
    BootHook * mNext;

    static BootHook * sHooks;

public:
    BootHook(Func * f) {
        mNext = sHooks;
        sHooks = this;
        mFunc = f;
    }

    static void exec() {
        for (BootHook * p = sHooks; p; p = p->mNext)
            p->mFunc();
    }
};

enum RadioSettings {
    RADIO_FASTMENUS = 0x0,
    RADIO_PANEL = 0x1,
    RADIO_SPEEDUP = 0x2,
    RADIO_GHOST = 0x3,
};

enum ScrollerSettings {
    SCROLLER_BOOT = 0x0,
    SCROLLER_INFO = 0x1
};

enum RadioFASTMENUS {
    RADIO_FASTMENUS_DISABLED = 0x0,
    RADIO_FASTMENUS_ENABLED = 0x1,
};

enum RadioPANEL {
    RADIO_PANEL_DISABLED = 0x0,
    RADIO_PANEL_LIGHT = 0x1,
    RADIO_PANEL_FULL = 0x2,
    RADIO_PANEL_OFFROAD = 0x3
};

enum RadioSPEEDUP {
    RADIO_SPEEDUP_DISABLED = 0x0,
    RADIO_SPEEDUP_ENABLED = 0x1
};

enum RadioGHOST {
    RADIO_GHOST_DISABLED = 0x0,
    RADIO_GHOST_ENABLED = 0x1
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

bool CheckButtonPressed(RealControllerHolder *controllerHolder, ControllerType controllerType, bool onlyCheckNewButtons, u32 button);
