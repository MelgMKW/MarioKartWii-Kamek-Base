#pragma once
#include <Kamek/kamek.hpp>
#include <UI/Ctrl/Action.hpp>
#include <UI/Screen/base.hpp>

class ToggleButtonChangeHandler;
class ToggleButton : public LayoutUIControl{
public:
    ToggleButton();
    virtual void Init();
    virtual void Update();
    void InitLayout(char *folderName, char *ctrName, char *variant, u32 localPlayerCount, bool r10, bool inacessible);
    void SetOnChangeHandler(ToggleButtonChangeHandler *handler); //80637d24
    void ToggleState(bool state); //changes the state without an animation
    void HandleSelect(u32 handlerId, u32 r5); //80638988
    void HandleDeselect(u32 handlerId, u32 r5); //80638a7c
    void HandleClick(u32 handlerId, u32 r5); //80638c64
    ControlActionHolder actionHolder; //0x174
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onSelectHandler; //80638988 1f8
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onDeselectHandler; //80638a7c 20C
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onClickHandler; //80638c64  220
    PtmfHolder_1A<Screen, void, ToggleButton*> *onChangeHandler; //234
    u32 localPlayerCount; //238
    bool state; //true for ON //23C

    u32 buttonId; //if multiple buttons //240
    nw4r::lyt::Pane *fuchi_pattern; //244
};

class ToggleButtonChangeHandler{
public:
    PtmfHolder_1A<Screen, void, ToggleButton*> onChangeHandler;
};
