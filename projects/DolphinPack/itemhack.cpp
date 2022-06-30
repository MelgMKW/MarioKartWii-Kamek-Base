#include <Kamek/kamek.hpp>
#include <Input/inputdata.hpp>
#include <Race/racedata.hpp>
#include <Race/raceinfo.hpp>
#include <Race/Item/itemholder.hpp>
#include <UI/menudata.hpp>
#include <project.hpp>

u32 currentItem;
const ItemId items[16] = {
    GOLDEN_MUSHROOM,
    THUNDER_CLOUD,
    MUSHROOM,
    BULLET_BILL,
    BOBOMB,
    GREEN_SHELL,
    FAKE_ITEM_BOX,
    MEGA_MUSHROOM,
    BLUE_SHELL,
    GREEN_SHELL,
    BANANA,
    RED_SHELL,
    BLOOPER,
    POW_BLOCK,
    LIGHTNING,
    NONE
};

void ItemHack(ItemHolderPlayer *player){
    u8 id = player->id;
    u8 hudSlotId = player->localPlayerNum;
    if (raceInfo->stage == 0) currentItem = -1;
    GameMode gamemode = raceData->main.scenarios[0].settings.gamemode;
    if((gamemode < 7 || gamemode > 10) && raceData->main.scenarios[0].players[id].playerType == PLAYER_REAL_LOCAL){
        if (player->localPlayerNum != 0xFFFFFFFF) {
            u32 controllerInfo = menuData->sub.controllerInfos[hudSlotId].controllerSlotAndTypeActive;
            ControllerType controllerType = ControllerType(controllerInfo & 0xFF);
            RealControllerHolder *controllerHolder = &inputData->realControllerHolders[((controllerInfo & 0xFF00) >> 8)-1];
            PlayerInventory *inventory = &player->inventory;

            u8 itemTableOffset = 0;
            u8 buttonsOffset = 0;
            u8 loopIterations = 8;
            u16 inputs = controllerHolder->inputStates[0].buttonRaw;
            
            UniversalButtons buttons[4];
            if (controllerType == WHEEL || controllerType == NUNCHUCK){
                loopIterations = 4;
                u32 switchButton1 = NUNCHUCK_B;
                u32 switchButton2 = NUNCHUCK_C;
                u32 switchButton3 = NUNCHUCK_B + NUNCHUCK_C;
                if(controllerType == WHEEL){
                    u32 switchButton1 = WHEEL_MINUS;
                    u32 switchButton2 = WHEEL_1;
                    u32 switchButton3 = WHEEL_MINUS + WHEEL_1;        
                } 
                if ((inputs & switchButton1) == switchButton1) itemTableOffset = 4;
                if ((inputs & switchButton2) == switchButton2) itemTableOffset = 8;
                if ((inputs & switchButton3) == switchButton3) itemTableOffset = 12;
                
            }
            else{
                buttonsOffset = 18;
                u32 switchButton1 = CLASSIC_ZL;
                if(controllerType == GCN) switchButton1 = GCN_Z;
                if ((inputs & switchButton1) == switchButton1) itemTableOffset = 8;
            }

            for (int i = 0; i < loopIterations; i++) if(CheckButtonPressed(controllerHolder, controllerType, true, 1<<(i+buttonsOffset))) currentItem = items[i + itemTableOffset];

            if (currentItem == GOLDEN_MUSHROOM){
                inventory->hasGolden = true;
                if (inventory->goldenTimer == 1){
                    inventory->hasGolden = false;
                    inventory->currentItemId = NONE;
                    inventory->currentItemCount = 0;
                    currentItem = -1;
                } 
            }

            if (currentItem != -1){
                inventory->currentItemId = (ItemId) currentItem;
                inventory->currentItemCount = currentItem == NONE ? 0 : 1;
                
            }
        }  
    }
    player->Update();
    return;
}
kmCall(0x8079994c, &ItemHack);
kmWrite32(0x80799CC8, 0x38000000);