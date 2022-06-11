#include <Kamek/kamek.hpp>
#include <Sound/AudioManager.hpp>
#include <UI/Screen/SpecificScreens/VSSettings.hpp>
#include <Race/playerholder.hpp>
#include <Race/kmp.hpp>
#include <CustomSettings.hpp>

//OSReport("MenuData: %x", sizeof());
extern const u8 buttonsPerRow[6] = {2, 3, 2, 3, 4, 4};
extern const u8 optionsPerScroller[6] = {3, 4, 4, 4, 4 ,4};
Screen *CreateSettingsScreen(){
    if (menuData->curScene->menuId == OPTIONS_MENU){
        return new(CustomSettingsPanel)(3, 2);
    }
    else{
        return new(VSSettingScreen);
    }
}
kmCall(0x80623e1c, &CreateSettingsScreen);
kmWrite32(0x80623e10,0x60000000); //nop the original new



void SizeTEST(nw4r::snd::SoundArchivePlayer *player, SimpleAudioMgr *mgr){
    OSReport("MenuData: %x", sizeof(RadioButtonControl));
    OSReport("MenuData: %x", sizeof(UpDownControl));
    OSReport("MenuData: %x", sizeof(TextUpDownValueControl));
    return;
}

kmBranch(0x806220ac, &SizeTEST);

    