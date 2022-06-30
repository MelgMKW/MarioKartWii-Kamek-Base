#include <Kamek/kamek.hpp>
#include <Sound/AudioManager.hpp>
#include <UI/Screen/SpecificScreens/VSSettings.hpp>
#include <Race/playerholder.hpp>
#include <Race/kmp.hpp>
#include <CustomSettings.hpp>
#include <CustomCtrlRaceBase.hpp>
#include <testing.hpp>

char *settingsBinName = "settings.bin";
//static CustomCtrlBuilder SOM(CountSOM, CreateSOM);
static CustomCtrlBuilder DEBUG(CountDEBUG, CreateDEBUG);
static CustomCtrlBuilder SOM(CountCustSOM, CreateCustSOM);
static CustomCtrlBuilder INFODISPLAY(CountINFODISPLAY, CreateINFODISPLAY);
u8 musicPackCount = 6;
s8 currentMusicPack = 0;
s8 currentMusicSlot = 33;

u32 CountCustSOM(){
    return 1;
}

void CreateCustSOM(Screen *screen, u32 index){
    CustomSOM *som = new(CustomSOM);
    screen->AddControl(index, som, 0); //If 0x9 screens, in solo localplayer = 1, 
    som->Load();
}

void CustomSOM::Load(){
    this->hudSlotId = 0;
    ControlLoader loader(this);
    loader.Load("game_image", "ctgprspeedometer", "CtrlSpeedometer_1_0", 0);
}

void CustomSOM::Init(){
    LayoutUIControl::Init();
}

void CustomSOM::OnUpdate(){
    float speed = playerHolder->players[this->getPlayerId()]->playerPointers->playerSub10->vehicleSpeed;
    wchar_t formattedspeed[0x30];
    swprintf(formattedspeed, 0x30, L"%3.1f", speed);
    ScreenText text;
    text.strings[0] = formattedspeed;
    this->SetMsgId(0x600, &text);
}



//OSReport("MenuData: %x", sizeof());
Screen *CreateSettingsScreen(){
    MenuType id = menuData->curScene->menuId;
    if ((id == OPTIONS_MENU)||(id == P1_WIFI)||(id == P1_WIFI_GLOBE_DISCONNECT)||(id == P1_WIFI_FROM_FIND_FRIEND)
    || (id == P2_WIFI) || (id == P2_WIFI_GLOBE_DISCONNECT)){
        u8 buttonsPerRow[8] = {2, 3, 2, 3, 4, 4, 4, 4};
        u8 optionsPerScroller[8] = {3, 4, 4, 4, 4 ,4, 4, 4};
        return new(CustomSettingsPanel)(3, 2, buttonsPerRow, optionsPerScroller);
    }
    else{
        return new(VSSettingScreen);
    }
}
kmCall(0x80623e1c, &CreateSettingsScreen);
kmWrite32(0x80623e10,0x60000000); //nop the original new

/*

void SizeTEST(nw4r::snd::SoundArchivePlayer *player, SimpleAudioMgr *mgr){
    OSReport("MenuData: %x", sizeof(Racedata));
    OSReport("MenuData: %x", sizeof(RacedataMain));
    OSReport("MenuData: %x", sizeof(RacedataScenario));
    OSReport("MenuData: %x", sizeof(RacedataPlayer));
    return;
}
kmBranch(0x806220ac, &SizeTEST);*/

extern void *LeCodeID;
int asm GetLECODEID(){ //get the offset to the lecode func from a bl, add it to the bl address, and call the function to get the id
    ASM(
        stwu sp, -0x10 (sp);
        mflr r0;
        stw r0, 0x14 (sp);
        lis r3, LeCodeID@h;
        ori r3, r3, LeCodeID@l;
        lwz r4, 0 (r3);
        rlwinm r4, r4, 0, 6, 29;
        add r3, r3, r4;
        mtctr r3;
        bctrl;
        mr r3, r4;
        lwz r0, 0x14 (sp);
        mtlr r0;
        addi sp, sp, 0x10;
        blr;
    )
}
