#pragma once
#include <Kamek/kamek.hpp>
#include <nw4r/snd/snd.hpp>
#include <System/camera.hpp>
#include <Input/inputdata.hpp>
#include <Sound/musichandler.hpp>
#include <Race/Item/itemobjproperties.hpp>
#include <UI/menudata.hpp>


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
extern "C"{
    void StoreFlapIntoSave(UnkType *saveDataManagerLicense, UnkType *timer, u32 r5, CourseId id);
}

bool CheckButtonPressed(RealControllerHolder *controllerHolder, ControllerType controllerType, bool onlyCheckNewButtons, u32 button);
bool CheckButtonCombination(RealControllerHolder *controllerHolder, ControllerType controllerType, bool onlyCheckNewButtons, UniversalButtons buttonMask);