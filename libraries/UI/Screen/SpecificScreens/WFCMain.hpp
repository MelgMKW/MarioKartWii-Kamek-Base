#pragma once
#include <Kamek/kamek.hpp>
#include <UI/Screen/base.hpp>

class FriendStatusButton : public PushButton {
    //no ctor, vtable 0x808bffc0
    virtual ~FriendStatusButton(); //8064b2fc
    u32 frames; //increments by 1 each frame, resets at 300
    u32 unknown_0x25C; //idk
}; //total size 0x25C

class WFCMainScreen : public Screen{
public:
    WFCMainScreen(); //8064b640
    virtual ~WFCMainScreen(); //vtable 0x808bff44 8064b850
    virtual int getNextScreen(); //0x8064d688
    virtual void OnInit(); //0x8064b910 
    virtual void OnLoad(); //0x8064bc90
    virtual int GetStaticInstanceAddr(); //0x8064d690
    void HandleSelectButton(PushButton *pushButton); //8064bf4c
    PtmfHolder_2A<WFCMainScreen, void, PushButton*, u32> onWorldwideClick; //0x44 vtable 808bffb4 8064bce8
    PtmfHolder_2A<WFCMainScreen, void, PushButton*, u32> onContinentalClick; //0x58 vtable 808bffb4 8064bdc4
    PtmfHolder_2A<WFCMainScreen, void, PushButton*, u32> onFriendsClick; //0x6C vtable 808bffb4 8064bea0
    PtmfHolder_2A<WFCMainScreen, void, PushButton*, u32> onBackButtonPress; //0x80 vtable 808bffb4 8064bee4 ctrlmenubackbutton
    PtmfHolder_1A<WFCMainScreen, void, PushButton*> onSelectHandler; //0x94 vtable 808bffb4 8064bf4c
    PtmfHolder_1A<WFCMainScreen, void, u32> onBackPress; //0xA8 vtable 808bffa8 8064bfb8
    CtrlMenuPageTitleText ctrlMenuPageTitleText; //0xBC
    PushButton worldwideButton; //0x230
    PushButton continentalButton; //0x484
    FriendStatusButton friendsButton; //0x6D8
    CtrlMenuBackButton backButton; //0x934
    CtrlMenuInstructionText bottomText; //0xb98
    ScreenActionController screenActionController; //0xd0c
    ScreenType nextScreenId; //when you press a button
};
//total size 0xf34