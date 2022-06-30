#pragma once
#include <Kamek/kamek.hpp>
#include <nw4r/snd/snd.hpp>
#include <System/camera.hpp>
#include <Input/inputdata.hpp>
#include <Sound/musichandler.hpp>
#include <Race/Item/itemobjproperties.hpp>
#include <UI/Ctrl/Ctrl.hpp>

extern "C"{
    void SetMatIcon(UnkType *r3, char *materialName, char *iconPaneName);
    int PlayBRSARSound(int r3, u32 soundId, u32 r5);
    void ChangeMusic(MusicHandler *musichandler, RaceState raceState);
    u32 LoadFromU8(UnkType *archive, u8 src, char *fileName, UnkType *r6);
    void CopyItemOBJPropertiesFromRelToTable(int *itemTable, ItemOBJProperties *itemObjProperties);
}

int GetLECODEID();

class CustomSOM : public CtrlRaceBase {
public:
    virtual void Init();
    virtual void OnUpdate();
    void Load();
};
u32 CountCustSOM();
void CreateCustSOM(Screen *screen, u32 index);