#include <Kamek/kamek.hpp>
#include <Race/racedata.hpp>
#include <Race/kmp.hpp>
#include <Race/playerholder.hpp>
#include <Race/Item/itemholder.hpp>
#include <UI/Screen/base.hpp>
#include <UI/Screen/ParentScreenBases.hpp>
#include <ParamsHolder.hpp>
#include <MyPack.hpp>

void patchCtrlRaceBaseCount(RaceScreen *raceScreen, u32 controlCount)
{
   u8 localPlayerCount = *(&raceData->main.scenarios[0].localPlayerCount);
   MenuType menuId = menuData->curScene->menuId;
   if(menuId >= WATCH_GHOST_FROM_CHANNEL && menuId <= WATCH_GHOST_FROM_MENU) localPlayerCount+= 1;
   bool hasDebug = ParamsHolder::sInstance->settings.radioSetting[RADIO_DEBUG] != RADIO_DEBUG_DISABLED;
   raceScreen->InitControlGroup(controlCount + localPlayerCount + hasDebug);
   return;
}

void createSOMandPanel(RaceScreen *raceScreen, u32 bitField){
   u8 localPlayerCount = *(&raceData->main.scenarios[0].localPlayerCount) ; //to make it 0 indexed
   MenuType menuId = menuData->curScene->menuId;
   if(menuId >= WATCH_GHOST_FROM_CHANNEL && menuId <= WATCH_GHOST_FROM_MENU) localPlayerCount+= 1;
   u32 firstIndex = raceScreen->controlGroup.controlCount - localPlayerCount - 1;
   for (int i = 0; i < localPlayerCount; i++){
      CtrlRaceSpeedo *som = new(CtrlRaceSpeedo);
      raceScreen->AddControl(firstIndex + i, som, 0); //If 0x9 screens, in solo localplayer = 1, 
      u8 speedoType = localPlayerCount;
      if(localPlayerCount == 3) speedoType = 4;
      char variant[0x20];
      snprintf(variant, 0x20, "CtrlRaceSpeedo_%1d_%1d", speedoType, i);
      som->Load(variant, i);
   }
   
   if(ParamsHolder::sInstance->settings.radioSetting[RADIO_DEBUG] != RADIO_DEBUG_DISABLED){
      CtrlRaceDebug *debug = new(CtrlRaceDebug);
      raceScreen->AddControl(firstIndex + localPlayerCount, debug, 0); //If 0x9 screens, in solo localplayer = 1,
      debug->Load();
   }
   
   raceScreen->InitCtrlRaceBase(bitField);
   return;
}
kmCall(0x808562d0, &patchCtrlRaceBaseCount);
kmCall(0x808562dc,&createSOMandPanel);

bool isPanelVisible;
void CtrlRaceDebug::Load(){
   this->hudSlotId = 0;
   ControlLoader loader(this);
   loader.Load("game_image", "debug", "CtrlRaceDebug", 0);
   return;
}

void CtrlRaceDebug::Init(){
   LayoutUIControl::Init();
   this->isHidden = isPanelVisible;
   return;
}

extern s8 currentMusicPack;
extern s8 musicPackCount;
void CtrlRaceDebug::OnUpdate(){
   this->UpdatePausePosition();
   u8 id = this->getPlayerId();
   RaceinfoPlayer *raceinfoPlayer = raceInfo->players[id];
   RealControllerHolder *controllerHolder = (RealControllerHolder*) raceinfoPlayer->controllerHolder;
   u16 curFrameInputs = controllerHolder->inputStates[0].buttonRaw;
   u16 prevFrameInputs = controllerHolder->inputStates[1].buttonRaw;

   ControllerType type = (ControllerType) (menuData->sub.controllerInfos[0].controllerSlotAndTypeActive & 0xFF);

   s8 iterator = 0x0;

   if (CheckButtonPressed(controllerHolder, type, true, BUTTON_DPAD_UP)) iterator = 1;
   else if (CheckButtonPressed(controllerHolder, type, true, BUTTON_DPAD_DOWN)) iterator = -1;

   bool otherButton = false;
   bool togglePanel = false;
   switch(type){
      case(CLASSIC):
         otherButton = (curFrameInputs & CLASSIC_Y) != 0;
         togglePanel = CheckButtonPressed(controllerHolder, type, true, BUTTON_MINUS);
         break;
      case(GCN):
         otherButton = (curFrameInputs & GCN_Z) != 0;
         togglePanel = CheckButtonPressed(controllerHolder, type, true, BUTTON_Y);
         break;
      default:
         otherButton = (curFrameInputs & NUNCHUCK_2) != 0;
         togglePanel = CheckButtonPressed(controllerHolder, type, true, BUTTON_MINUS);

   }
    if(otherButton && iterator != 0){
      currentMusicPack = (iterator + currentMusicPack) % (musicPackCount);
      if (currentMusicPack < 1) currentMusicPack += musicPackCount;
   }

   if (togglePanel) isPanelVisible = !isPanelVisible;
   this->isHidden= !isPanelVisible;
   if(isPanelVisible){
      u8 respawn = raceinfoPlayer->respawn;
      u8 cp = raceinfoPlayer->checkpoint;
      u8 kcp = raceinfoPlayer->currentKCP;
      float raceComp = raceinfoPlayer->raceCompletion;
      u8 ITPT = itemHolder->players[id].itemPoint.ITPT;
      
      PlayerPointers *pointers = &playerHolder->players[id]->pointers;
      PlayerSound *sound = pointers->playerSound;
      u16 kclFlag = sound->kclFlag;
      u16 kclVariant = sound->variant;

      wchar_t *trick = L"Can't  Trick";
      u32 collisionType = pointers->wheels[0]->wheelPhysics->hitboxGroup->collisionData.types;
      if (kclFlag == 0x7 || kclFlag == 0x6 || (collisionType & 0x8)) trick = L"Can  Trick";

      wchar_t *drop = L"Can  Drop";
      u16 billSettings = KMP::controller->itemPoint->pointArray[ITPT]->raw->setting2;
      if (billSettings & 1) drop = L"Can't Drop";

      wchar_t debugPanel[0x200];
      const wchar_t *format;
      u32 length = 0;
      if (ParamsHolder::sInstance->settings.radioSetting[RADIO_DEBUG] == RADIO_DEBUG_LIGHT){
         format = L"Respawn %2d\nCurCP    %3d\nCurKCP   %2d\nRace%%    %01.3f\nITPT      %3d\nKCL %02x %02x\n%ls\n%ls\nMP %d";
         length = swprintf(debugPanel, 0x200, format, respawn, cp, kcp, raceComp, ITPT, kclFlag, kclVariant, trick, drop, currentMusicPack);
      }
      else if (ParamsHolder::sInstance->settings.radioSetting[RADIO_DEBUG] == RADIO_DEBUG_FULL){
         Vec3 *position = &pointers->playerGraphics->playerPhysicsHolder->playerPhysics->position;
         format = L"X %5.0f\nY %5.0f\nZ %5.0f\nRespawn %2d\nCurCP    %3d\nCurKCP   %2d\nRace%%    %01.3f\nITPT      %3d\nKCL %02x %02x\n%ls\n%ls\nMP %d";
         length = swprintf(debugPanel, 0x200, format, position->x, position->y, position->z, 
         respawn, cp, kcp, raceComp, ITPT, kclFlag, kclVariant, trick, drop, currentMusicPack);
      }


      
      
      for (u32 i = 0; i<length; i++){
         if (debugPanel[i] >= 0x30 && debugPanel[i] <= 0x39) debugPanel[i] += 0x2430;
      }

      ScreenText text;
      text.strings[0] = debugPanel;
      this->SetMsgId(0x600, &text);
   }
   return;
}

/* SPEEDOMETER */
void CtrlRaceSpeedo::Load(char* variant, u8 id){ //blatant copy
   this->hudSlotId = id;
   ControlLoader loader(this);
   const char* anims[16] = {"eHundreds", "texture_pattern_0_9_0", NULL,
    "eTens", "texture_pattern_0_9_1", NULL,
    "eUnits", "texture_pattern_0_9_2", NULL,
    "eDot", "texture_pattern_0_9_3",NULL,
    "eDecimals", "texture_pattern_0_9_4",NULL, NULL};

   loader.Load("game_image", "speedometer", variant, anims);
   PaneGroup *paneGroup = this->AnimationThing.PaneGroupById(0);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->AnimationThing.PaneGroupById(1);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->AnimationThing.PaneGroupById(2);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->AnimationThing.PaneGroupById(3);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->AnimationThing.PaneGroupById(4);
   paneGroup->DisplayAnimation(0, 0.0f);
   return;
}

void CtrlRaceSpeedo::Init(){
   this->HudSlotColorEnabled("speed0", true);
   this->HudSlotColorEnabled("speed1", true);
   this->HudSlotColorEnabled("speed2", true);
   this->HudSlotColorEnabled("speed3", true);
   this->HudSlotColorEnabled("speed4", true);
   this->HudSlotColorEnabled("kmh", true);
   LayoutUIControl::Init();
   return;
}

void CtrlRaceSpeedo::OnUpdate(){
   this->UpdatePausePosition();
   u8 id = this->getPlayerId();
  
   PlayerPointers *pointers = &playerHolder->players[id]->pointers;
   PlayerPhysicsHolder *physicsHolder = pointers->playerGraphics->playerPhysicsHolder;
   PlayerPhysics *physics = physicsHolder->playerPhysics;
   
   Vec3 sum;
   PSVECAdd(&physics->engineSpeed, &physics->speed2, &sum);
   PSVECAdd(&physics->speed3, &sum, &sum);
   float speed = PSVECMag(&sum);
   float speedCap = pointers->playerSub10->hardSpeedLimit;
   if(speed > speedCap) speed = speedCap;


   u32 engineSpeed = (u32) (speed * 10.0f);

   float decimals = (float) (engineSpeed % 10 / 1); 
   float dot = 11.0f;
   float units = (float) (engineSpeed % 100 / 10);  
   float tens; tens = (float) (engineSpeed % 1000 / 100);
   float hundreds = (float) (engineSpeed % 10000 / 1000);
   if (engineSpeed < 100){
      hundreds = units;
      tens = dot;
      units = decimals;
      dot = 10.0f;
      decimals = 10.0f;
   }
   else if (engineSpeed < 1000) {
      hundreds = tens;
      tens = units;
      units = dot;
      dot = decimals;
      decimals = 10.0f;
   }
   PaneGroup *paneGroup = this->AnimationThing.PaneGroupById(3);
   paneGroup->DisplayAnimation(0, decimals);

   paneGroup = this->AnimationThing.PaneGroupById(4);
   paneGroup->DisplayAnimation(0, dot);

   paneGroup = this->AnimationThing.PaneGroupById(2); //third digit
   paneGroup->DisplayAnimation(0, units);

   paneGroup = this->AnimationThing.PaneGroupById(1); //second digit
   paneGroup->DisplayAnimation(0, tens);

   paneGroup = this->AnimationThing.PaneGroupById(0); //first digit
   paneGroup->DisplayAnimation(0, hundreds);
   return;
}
