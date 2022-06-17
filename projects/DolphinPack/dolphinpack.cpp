#include <Kamek/kamek.hpp>
#include <CustomSettings.hpp>
#include <dolphinpack.hpp>
#include <project.hpp>
#include <CustomCtrl.hpp>

char *settingsBinName = "TestPack.bin";
static CustomCtrlBuilder SOM(CountSOM, CreateSOM);
static CustomCtrlBuilder OFFROAD(CountOFFROAD, CreateOFFROAD);
static CustomCtrlBuilder DEBUG(CountDEBUG, CreateDEBUG);

Screen *CreateSettingsScreen(){
    MenuType id = menuData->curScene->menuId;
    if ((id == OPTIONS_MENU) ||(id == P1_WIFI_GLOBE_DISCONNECT)||(id == P1_WIFI_FROM_FIND_FRIEND)
    || (id == P2_WIFI) || (id == P2_WIFI_GLOBE_DISCONNECT)){
        u8 buttonsPerRow[8] = {2, 4, 2, 2, 4, 4, 4 ,4};
        u8 optionsPerScroller[8] = {3, 4, 4, 4, 4 , 4, 4, 4};
        return new(CustomSettingsPanel)(4, 2, buttonsPerRow, optionsPerScroller);
    }
    else{
        return new(VSSettingScreen);
    }
}
kmCall(0x80623e1c, &CreateSettingsScreen);
kmWrite32(0x80623e10,0x60000000); //nop the original new


extern const u8 buttonsPerRow[6] = {2, 4, 2, 2, 4, 4};
extern const u8 optionsPerScroller[6] = {3, 4, 4, 4, 4 ,4};

void FasterMenusOnTransition(MenuData *menuData, u32 delay, u32 r5){
    ParamsHolder *params = ParamsHolder::sInstance;
    float transitionDelay = 176.0f;
    if (params->settings.radioSetting[RADIO_FASTMENUS] == RADIO_FASTMENUS_ENABLED){
        delay = 0;
        transitionDelay -= transitionDelay;
    }
    screenTransitionDelay = transitionDelay;
    menuData->StartTransition(delay, r5);
}
kmCall(0x80602510, &FasterMenusOnTransition);

void FasterMenusOnBoot(){
    ParamsHolder *params = ParamsHolder::sInstance;
    float transitionDelay = 176.0f;
    if (params->settings.radioSetting[RADIO_FASTMENUS] == RADIO_FASTMENUS_ENABLED){
        transitionDelay -= transitionDelay;
    }
    screenTransitionDelay = transitionDelay;
};
static BootHook FasterMenus(FasterMenusOnBoot);

s32 GhostAlwaysSaves(s32 ghostTop5Position){
    ParamsHolder *params = ParamsHolder::sInstance;
    if (params->settings.radioSetting[RADIO_GHOST] == RADIO_GHOST_ENABLED) return 0;
    return ghostTop5Position;
}
kmBranch(0x805491e0, &GhostAlwaysSaves); //ghost always saves
kmWrite32(0x80857aa0, 0x60000000); //disable ghost cannot be saved message