#include <Kamek/kamek.hpp>
#include <CustomSettings.hpp>
#include <MyPack.hpp>
#include <project.hpp>
#include <CustomCtrl.hpp>

char *settingsBinName = "MySettings.bin";
static CustomCtrlBuilder SOM(CountSOM, CreateSOM);
static CustomCtrlBuilder MYDEBUG(CountMYDEBUG, CreateMYDEBUG);
static CustomCtrlBuilder OFFROAD(CountOFFROAD, CreateOFFROAD);


Screen *CreateSettingsScreen(){
    MenuType id = menuData->curScene->menuId;
    if ((id == OPTIONS_MENU) ||(id == P1_WIFI_GLOBE_DISCONNECT)||(id == P1_WIFI_FROM_FIND_FRIEND)
    || (id == P2_WIFI) || (id == P2_WIFI_GLOBE_DISCONNECT)){
        u8 buttonsPerRow[8] = {2, 4, 2, 3, 4, 4, 4, 4};
        u8 optionsPerScroller[8] = {3, 4, 4, 4, 4 ,4, 4, 4};
        return new(CustomSettingsPanel)(3, 2, buttonsPerRow, optionsPerScroller);
    }
    else{
        return new(VSSettingScreen);
    }
}
kmCall(0x80623e1c, &CreateSettingsScreen);
kmWrite32(0x80623e10,0x60000000); //nop the original new
kmWrite32(0x805491B4, 0x41820018); //ghost always saves

extern const u8 buttonsPerRow[6] = {2, 4, 2, 3, 4, 4};
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
