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