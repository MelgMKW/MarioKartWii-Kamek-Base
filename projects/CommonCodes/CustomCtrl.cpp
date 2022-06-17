#include <Kamek/kamek.hpp>
#include <Race/racedata.hpp>
#include <Race/kmp.hpp>
#include <Race/playerholder.hpp>
#include <Race/Item/itemholder.hpp>
#include <UI/Screen/base.hpp>
#include <UI/Screen/ParentScreenBases.hpp>
#include <ParamsHolder.hpp>
#include <CustomCtrl.hpp>
#include <project.hpp>

CustomCtrlBuilder *CustomCtrlBuilder::sHooks = NULL;

void CreateCustomRaceCtrls(RaceScreen *raceScreen, u32 controlCount)
{
   u32 customctrlCount = CustomCtrlBuilder::Count();
   raceScreen->InitControlGroup(controlCount + customctrlCount);
   CustomCtrlBuilder::CreateCustomCtrls(raceScreen, controlCount);
   return;
}
kmCall(0x808562d0, &CreateCustomRaceCtrls);

u32 CountSOM(){
   u32 localPlayerCount = raceData->main.scenarios[0].localPlayerCount;
   MenuType menuId = menuData->curScene->menuId;
   if(menuId >= WATCH_GHOST_FROM_CHANNEL && menuId <= WATCH_GHOST_FROM_MENU) localPlayerCount+= 1;
   return localPlayerCount;
}
void CreateSOM(Screen *screen, u32 index){
   u32 localPlayerCount = raceData->main.scenarios[0].localPlayerCount;
   MenuType menuId = menuData->curScene->menuId;
   if(menuId >= WATCH_GHOST_FROM_CHANNEL && menuId <= WATCH_GHOST_FROM_MENU) localPlayerCount+= 1;
   u8 speedoType = (localPlayerCount == 3) ? 4 : localPlayerCount;
   for (int i = 0; i < localPlayerCount; i++){
      CtrlRaceSpeedo *som = new(CtrlRaceSpeedo);
      screen->AddControl(index + i, som, 0); //If 0x9 screens, in solo localplayer = 1, 
      char variant[0x20];
      snprintf(variant, 0x20, "CtrlRaceSpeedo_%1d_%1d", speedoType, i);
      som->Load(variant, i);
   }
}

u32 CountDEBUG(){
   u32 panelSetting = ParamsHolder::sInstance->settings.radioSetting[RADIO_PANEL];
   return ((panelSetting == RADIO_PANEL_LIGHT) || (panelSetting == RADIO_PANEL_FULL)) ? 1 : 0;
}
void CreateDEBUG(Screen *screen, u32 index){
      CtrlRaceDebug *debug = new(CtrlRaceDebug);
      screen->AddControl(index, debug, 0); //If 0x9 screens, in solo localplayer = 1,
      debug->Load();
}

u32 CountOFFROAD(){
   u32 panelSetting = ParamsHolder::sInstance->settings.radioSetting[RADIO_PANEL];
   return (panelSetting == RADIO_PANEL_OFFROAD) ? 1 : 0;
}
void CreateOFFROAD(Screen *screen, u32 index){
      CtrlRaceOffroad *offroad = new(CtrlRaceOffroad);
      screen->AddControl(index, offroad, 0); //If 0x9 screens, in solo localplayer = 1,
      offroad->Load();
}

u32 CountINFODISPLAY(){
   u32 gamemode = raceData->main.scenarios[0].settings.gamemode;
   if ((gamemode == MODE_VS_RACE) || (gamemode == MODE_PUBLIC_VS)|| (gamemode == MODE_PRIVATE_VS)) return 1;
   return 0;
}
void CreateINFODISPLAY(Screen *screen, u32 index){
   CtrlRaceTrackInfoDisplay *info = new(CtrlRaceTrackInfoDisplay);
   screen->AddControl(index, info, 0);
   info->Load();

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
   PaneGroup *paneGroup = this->animator.PaneGroupById(0);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->animator.PaneGroupById(1);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->animator.PaneGroupById(2);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->animator.PaneGroupById(3);
   paneGroup->DisplayAnimation(0, 0.0f);

   paneGroup = this->animator.PaneGroupById(4);
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
   PaneGroup *paneGroup = this->animator.PaneGroupById(3);
   paneGroup->DisplayAnimation(0, decimals);

   paneGroup = this->animator.PaneGroupById(4);
   paneGroup->DisplayAnimation(0, dot);

   paneGroup = this->animator.PaneGroupById(2); //third digit
   paneGroup->DisplayAnimation(0, units);

   paneGroup = this->animator.PaneGroupById(1); //second digit
   paneGroup->DisplayAnimation(0, tens);

   paneGroup = this->animator.PaneGroupById(0); //first digit
   paneGroup->DisplayAnimation(0, hundreds);
   return;
}


bool isDebugPanelVisible = false;
void CtrlRaceDebug::Load(){
   this->hudSlotId = 0;
   ControlLoader loader(this);
   if (ParamsHolder::sInstance->settings.radioSetting[RADIO_PANEL] == RADIO_PANEL_LIGHT){
      loader.Load("game_image", "DebugLight", "DebugLight", 0);
   }
   else loader.Load("game_image", "DebugFull", "DebugFull", 0);

   return;
}

void CtrlRaceDebug::Init(){
   LayoutUIControl::Init();
   this->isHidden = isDebugPanelVisible;
   return;
}

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

   bool togglePanel = false;
   switch(type){
      case(CLASSIC):
         togglePanel = CheckButtonPressed(controllerHolder, type, true, BUTTON_MINUS);
         break;
      case(GCN):
         togglePanel = CheckButtonPressed(controllerHolder, type, true, BUTTON_Y);
         break;
      default:
         togglePanel = CheckButtonPressed(controllerHolder, type, true, BUTTON_MINUS);
   }

   if (togglePanel) isDebugPanelVisible = !isDebugPanelVisible;
   this->isHidden= !isDebugPanelVisible;
   if(isDebugPanelVisible){
      u8 respawn = raceinfoPlayer->respawn;
      u8 cp = raceinfoPlayer->checkpoint;
      u8 kcp = raceinfoPlayer->currentKCP;
      u8 maxkcp = raceinfoPlayer->maxKCP;
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
      if (ParamsHolder::sInstance->settings.radioSetting[RADIO_PANEL] == RADIO_PANEL_LIGHT){
         format = L"Respawn %2d\nCurCP    %3d\nCurKCP   %2d\nRace%%    %01.3f\nITPT      %3d\nKCL %02x %02x\n%ls\n%ls";
         length = swprintf(debugPanel, 0x200, format, respawn, cp, maxkcp, raceComp, ITPT, kclFlag, kclVariant, trick, drop);
      }
      else if (ParamsHolder::sInstance->settings.radioSetting[RADIO_PANEL] == RADIO_PANEL_FULL){
         Vec3 *position = &pointers->playerGraphics->playerPhysicsHolder->playerPhysics->position;
         format = L"X %5.0f\nY %5.0f\nZ %5.0f\nRespawn %2d\nCurCP    %3d\nCur/MaxKCP %2d %2d\nRace%%    %01.3f\nITPT      %3d\nKCL %02x %02x\n%ls\n%ls";
         length = swprintf(debugPanel, 0x200, format, position->x, position->y, position->z, respawn, cp, kcp, maxkcp, raceComp, ITPT, kclFlag, kclVariant, trick, drop);
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


void CtrlRaceOffroad::Load(){
   this->hudSlotId = 0;
   ControlLoader loader(this);
   loader.Load("game_image", "OffroadInfo", "OffroadInfo", 0);
   return;
}

void CtrlRaceOffroad::Init(){
   LayoutUIControl::Init();
   u8 id = this->getPlayerId();
   this->offroadFrames = 0;
   this->minOffSpeed = playerHolder->players[id]->pointers.playerSub10->baseSpeed;
   return;
}

void CtrlRaceOffroad::OnUpdate(){
   this->UpdatePausePosition();
   u8 id = this->getPlayerId();
   PlayerPointers *pointers = &playerHolder->players[id]->pointers;
   
   PlayerSub10 *sub10 = pointers->playerSub10;
   float speedMult = sub10->kclWheelSpeedFactor * 100.0f;

   float vehicleSpd = fabs(sub10->vehicleSpeed);
   u16 kclFlag = pointers->playerSound->kclFlag;
   if ((0x2 <= kclFlag) &&(kclFlag <= 0x4) && (vehicleSpd < sub10->baseSpeed) && (speedMult < 100.0f) && (menuData->curScene->isPaused == false)){
      this->offroadFrames++;
      if (vehicleSpd < this->minOffSpeed) this->minOffSpeed = vehicleSpd;
   }

   wchar_t *format = L"Off Frames %4d\nKCLFlag Mul %-4.1f%%\nMin Off Speed %4.1f\n";
   wchar_t offroadPanel[0x100];
   u32 length = swprintf(offroadPanel, 0x100, format, this->offroadFrames, speedMult, this->minOffSpeed);
   for (u32 i = 0; i<length; i++){
      if (offroadPanel[i] >= 0x30 && offroadPanel[i] <= 0x39) offroadPanel[i] += 0x2430;
   }
   ScreenText text;
   text.strings[0] = offroadPanel;
   u32 bmgId = (this->offroadFrames > 0) ? 0x602 : 0x601;
   this->SetMsgId(bmgId, &text);
}


void CtrlRaceTrackInfoDisplay::Load(){
   this->hudSlotId = 0;
   ControlLoader loader(this);
   loader.Load("game_image", "CTInfo", "Info", 0);
   this->textBox_00 = this->layout.GetPaneByName("TextBox_00");
   //u32 id = GetLECODEID();
   u32 id = raceData->main.scenarios[0].settings.courseId;
   u32 bmgId = 0x7000 + id;
   this->SetMsgId(bmgId, 0);
}