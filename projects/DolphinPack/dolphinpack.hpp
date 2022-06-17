#pragma once
#include <Kamek/kamek.hpp>
#include <System/system.hpp>
#include <UI/Ctrl/Ctrl.hpp>
#include <Race/lakitucontroller.hpp>
#include <Sound/musichandler.hpp>


bool CheckButtonPressed(RealControllerHolder *controllerHolder, ControllerType controllerType, bool onlyCheckNewButtons, u32 button);

extern "C"{
    void StoreFlapIntoSave(UnkType *saveDataManagerLicense, UnkType *timer, u32 r5, CourseId id);
}
extern float screenTransitionDelay;