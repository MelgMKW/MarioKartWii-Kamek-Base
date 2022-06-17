#pragma once
#include <Kamek/kamek.hpp>
#include <System/system.hpp>
#include <UI/Ctrl/Ctrl.hpp>
#include <Race/lakitucontroller.hpp>
#include <Sound/musichandler.hpp>

u32 CountMYDEBUG();
void CreateMYDEBUG(Screen *screen, u32 index);


extern "C"{
    void StoreFlapIntoSave(UnkType *saveDataManagerLicense, UnkType *timer, u32 r5, CourseId id);
}
extern float screenTransitionDelay;

class CtrlRaceMyDebug : public CtrlRaceBase {
public:
    virtual void Init();
    virtual void OnUpdate();
    void Load();
};
