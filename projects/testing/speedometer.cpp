#include <UI/Screen/base.hpp>
#include <Race/racedata.hpp>
#include <Race/playerholder.hpp>
#include <UI/Screen/ParentScreenBases.hpp>
#include <testing.hpp>


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
}

void CtrlRaceSpeedo::Init(){
   this->HudSlotColorEnabled("speed0", true);
   this->HudSlotColorEnabled("speed1", true);
   this->HudSlotColorEnabled("speed2", true);
   this->HudSlotColorEnabled("speed3", true);
   this->HudSlotColorEnabled("speed4", true);
   this->HudSlotColorEnabled("kmh", true);
   LayoutUIControl::Init();
}

void CtrlRaceSpeedo::OnUpdate(){
   this->UpdatePausePosition();
   u8 id = this->getPlayerId();
   u32 engineSpeed = (u32) (fabs((playerHolder->players[id]->pointers.playerSub10->vehicleSpeed)) * 10.0);
  
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
}

void patchCtrlRaceBaseCount(RaceScreen *raceScreen, u32 controlCount)
{
   u8 localPlayerCount = *(&raceData->main.scenarios[0].localPlayerCount);
   raceScreen->InitControlGroup(controlCount + localPlayerCount);
}

void createSOM(RaceScreen *raceScreen, u32 bitField){
   u8 localPlayerCount = *(&raceData->main.scenarios[0].localPlayerCount) ; //to make it 0 indexed
   u32 firstIndex = raceScreen->controlGroup.controlCount - localPlayerCount;
   for (int i = 0; i < localPlayerCount; i++){
      CtrlRaceSpeedo *som = new(CtrlRaceSpeedo);
      raceScreen->AddControl(firstIndex + i, som, 0); //If 0x9 screens, in solo localplayer = 1, 
      u8 speedoType = localPlayerCount;
      if(localPlayerCount == 3) speedoType = 4;
      char variant[0x20];
      snprintf(variant, 0x20, "CtrlRaceSpeedo_%1d_%1d", speedoType, i);
      som->Load(variant, i);
   }
   raceScreen->InitCtrlRaceBase(bitField);
}

kmCall(0x808562d0, &patchCtrlRaceBaseCount);
kmCall(0x808562dc,&createSOM);

//kmBranch(0x80856660, &SizeTest);
