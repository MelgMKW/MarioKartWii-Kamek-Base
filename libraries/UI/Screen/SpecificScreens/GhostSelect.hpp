#pragma once

#include <Kamek/kamek.hpp>
#include <UI/Screen/base.hpp>
#include <UI/Screen/ParentScreenBases.hpp>
#include <UI/Screen/SpecificScreens/SharedStructs.hpp>
#include <UI/Screen/SpecificScreens/GhostManager.hpp>
#include <Misc/Mii.hpp>

class GhostInfoControl : public LayoutUIControl {
public:
    GhostInfoControl(); //0x805e274c
    virtual ~GhostInfoControl(); //0x805e27dc vtable 808b92c0
    MiiGroup miiGroup; //0x174
    //0x20c
    u8 unknown_0x20C[0x234-0x20c];
    u32 unknown_0x234; //set to 0
    u16 unknown_0x238; //set to 0
    u16 unknown_0x23A; //set to 0
    float unknown_0x23C;
    float unknown_0x240;
    u16 unknown_0x244;
    u16 unknown_0x246;
    u32 unknown_0x248; //gets set to 0x00080000 in the ctor
}; //total size 0x24C


class GhostSelectScreen : public Screen { //808bebd8
public:
    GhostSelectScreen(); //0x806395ec vtable 808bec2c
    virtual ~GhostSelectScreen(); //0x8063982c
    virtual int getNextScreen(); //8063a4b8
    virtual void OnInit(); //80639908
    virtual void OnLoad(); //80639bd0
    virtual void OnActivate(); //80639d34
    virtual void OnResume(); //80639d6c
    virtual int GetStaticInstanceAddr(); //8063a4c0

    void HandleWatchReplayPress(PushButton *button, u32 handlerId);
    void HandleRightArrowPress(SheetSelectControl *control, u32 handlerId); //8063a1a4
    void HandleLeftArrowPress(SheetSelectControl *control, u32 handlerId); //8063a2a4
    void HandleBackPress(u32 handlerId); //8063a444
    ScreenActionController screenactionController; //0x44
    CtrlMenuPageTitleText ctrlMenuPageTitleText; //0x268
    GhostInfoControl ghostInfo; //loads GhostInfo brctr
    GhostInfoControl ghostInfo2; //first for easy, 2nd for expert?
    SheetSelectControl arrowPairControl; //0x874
    LayoutUIControl pageNumber;
    PushButton challengeGhostButton;
    PushButton watchReplayButton;
    PushButton soloTTButton;
    CtrlMenuBackButton backButton; //ends at 1880
    GhostInfoControl *info;
    GhostInfoControl *info2;
    u32 selectedButtonId; //0x1888
    PtmfHolder_2A<Screen, void, SheetSelectControl*, u32> onRightArrowPressHandler;  //8063a1a4 0x188c
    PtmfHolder_2A<Screen, void, SheetSelectControl*, u32> onLeftArrowPressHandler;  //8063a2a4 0x18a0
    PtmfHolder_2A<Screen, void, PushButton*, u32> onChallengeGhostClickHandler;  //8063a3a0 0x18B4
    PtmfHolder_2A<Screen, void, PushButton*, u32> onWatchReplayClickHandler;  //8063a3c0 0x18C8
    PtmfHolder_2A<Screen, void, PushButton*, u32> onSoloTimeTrialClickHandler; //8063a3e0 0x18DC
    PtmfHolder_2A<Screen, void, CtrlMenuBackButton*, u32> onBackButtonClickHandler; //8063a444 0x18F0
    PtmfHolder_1A<Screen, void, u32> onBackPress; //8063a400 0x1904
    GhostList *ghostList; //from screen 0xA7
    u32 page;
    ScreenType nextScreenId; //0x1920
}; //total size 0x1924