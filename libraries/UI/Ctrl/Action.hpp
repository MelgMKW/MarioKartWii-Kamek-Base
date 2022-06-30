#pragma once
#include <Kamek/kamek.hpp>
#include <nw4r/lyt/lyt.hpp>
#include <system/egg.hpp>
#include <UI/Ctrl/Ctrl.hpp>

enum ScreenInput{ //each value represent an action, itself represented by an actionHandler in PushButton's screenElementActionHolder and a bool in the same struct 
    FORWARD_PRESS,
    BACK_PRESS,
    START_PRESS,
    UP_PRESS,
    DOWN_PRESS,
    LEFT_PRESS,
    RIGHT_PRESS,
    UNK_PRESS,
    SWITCH_PRESS, //minus and Z
    UNK_PRESS_0x9 = 0x9
};
class Screen;
class InteractableScreen;
class ControlActionHolder;

class ButtonInfo { //actually inherited from something else 808b9a80
public:
    ButtonInfo(); //inlined for screenactionHandler, 805eee24 for screens
    virtual ~ButtonInfo(); //805ef848 vtable 808b9a30
    virtual void func_0xC(); 
    virtual void func_0x10();
    virtual void func_0x14();
    u32 forwardPressTime; //2 or A
    u32 backPressTime; //1 or B
    u32 startPressTime; //+ or start
    u32 upPressTime;
    u32 downPressTime;    
    u32 leftPressTime;
    u32 rightPressTime;
    u32 unknown_0x20;
    u32 spectateSwitchPressTime; //- + x + z
    u8 unknown_0x28[0x34-0x28];
}; //total size 0x34

class ScreenActionHandler{
public:
    ScreenActionHandler(); //805f0a98
    ControlActionHolder *curActionHolder;
    u32 unknown_0x4; //passed as an arg
    ButtonInfo buttonInfo;
    u32 enabledHandlers;
    u32 resetTime;
    u8 unknown_0x44[0x5C-0x44];
}; //total size 0x5C

class ScreenActionController{
public:
    ScreenActionController();
    ~ScreenActionController(); //805f0bd4 vtable 0x808b99e8
    virtual void Init(u32 r4, bool r5);
    virtual void OnLoad(); //805f1cf4 //0x20
    virtual void Update(); 
    void AddActionHolder(ControlActionHolder *holder);
    void SetHandler(ScreenInput id, ptmfHolder *handler, bool enable, bool r7);
    void Load(); // 805eed70
    u32 playerCount;
    u8 unknown_0x8[0xD-0x8];
    bool isMultiplayer;
    u8 unknown_0xE[2]; //padding?
    EGG::List actionHolderList; //0x10
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *handlers[9]; //805f15cc, first arg is actionHandler id
    u8 unknown_0x40[0x54-0x40];
    ScreenActionHandler actionHandlers[5]; 
    UnkType calcDistanceFunc;
    
};//total size 0x224

class ActionSelectHandler{
public:
    ActionSelectHandler(); //805f058c
    nw4r::lyt::Pane *pane;
    Vec3 vec3_0x10;
    Vec2 vec2_0x1C;
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *onSelectHandler;  //805f15f8
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *onDeselectHandler;
    u32 unknown_0x20;
    u8 unknown_0x24[0x28-0x24];
}; //total size 0x28

class ControlActionHolder{
public:
    ControlActionHolder(); //805efaf8 
    void Init(u32 childCount, bool r5, bool inacessible);
    void SetHandler(ScreenInput id, ptmfHolder *handler, bool r6);
    void SetBorderColours(RGBA16 *firstHalf, RGBA16 *secondHalf); //805f03dc
    void SetLocalPlayerCount(u32 localPlayerCount); //805efd14

    ControlActionHolder *nextHolder; 
    ControlActionHolder *prevHolder;
    virtual void unk_vtable(); //808b9a20
    ActionSelectHandler selectHandler; //0xC
    ActionSelectHandler *childHandlers; //805efc98 array here 0x34
    u32 childCount; //0x38
    ScreenActionController *parentController;
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *actionHandlers[9]; //each corresponds to a ScreenInput, size based on ctor
    bool actionsSet[9]; //same
    u8 unknown_0x6D[3]; //padding?
    u32 localPlayerCount;
    bool unknown_0x74; //r5 from init
    bool inacessible; //0x75
    u8 unknown_0x76[2]; //padding?
    u32 borderColoursArrayIndex; //80895cc0 position 0, has colours for hud slots for 1st 4 slots 805efef4 index is set here
    u32 unknown_0x7C; //probably same thing as above
    u32 distanceOfSomeSort; //converted from a float
}; //total size 0x84

class SheetSelectButton : public LayoutUIControl {
    SheetSelectButton(); //inlined but at 80636470vtable 0x808be998
    virtual ~SheetSelectButton(); //0x80636564
    ControlActionHolder actionHolder;
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onSelectHandler; //vtable = 0x808be9d4 function = 80636d08 on itself
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onDeselectHandler; //vtable = 0x808be9d4 function = 80636fc4 on itself 0x20C
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onScrollHandler; //vtable = 0x808be9d4 function = 806371ac on itself
    u32 arrowSide; //0x0 for left, 0x1 for right
    u32 playerCount; //copied straight from arrowpaircontrol
    bool isVisible;
    u8 unknown_0x23d[0x240-0x23d]; //might be padding
    u32 *fuchi_pattern;
    u32 *rootPane;
}; //total size 0x248

class SheetSelectControl : public UIControl { //arrowpair
    SheetSelectControl(); //80635ec4 vtable 0x808be950
    virtual ~SheetSelectControl(); //8063607c
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *rightArrowHandler;
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *leftArrowHandler;
    u32 playerCount;
    u8 unknown_0xA4[0xa8-0xa4];
    SheetSelectButton leftArrow; //0xa8
    SheetSelectButton rightArrow;
}; //total size 0x538

class PushButton : public LayoutUIControl{
public:
    PushButton(); //vtable 808B776C
    virtual ~PushButton(); //0x805bd4b0
    virtual void Init(); //0x805bdbe0
    virtual void Update(); //0x805bdd98
    virtual int GetStaticInstanceAddr(); //0x805be610
    virtual char* getClassName(); //0x805bd39c
    virtual void OnButtonInit(); //0x805bd710
    virtual void OnSelect(); //0x805be12c
    virtual void OnDeselect(); //0x805be354
    virtual void OnClick(); //0x805be42c
    void SelectButton(u32 localPlayerId);
    void SelectFocus();
    void InitButton(char *folderName, char *ctrName, char *variant, u32 localPlayerCount, u32 r8, bool inaccessible); //805bd518
    void SetOnClickHandler(PtmfHolderBase_2A<void, PushButton*, u32> *handler, u8 r5);
    void SetOnSelectHandler(PtmfHolderBase_1A<void, PushButton*> *handler);
    float GetAnimationFrameSize(); //0x805bdf88
    void HandleClick(u32 handlerId, u32 r5);

    ControlActionHolder actionHolder;
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onSelectHandlerObj; //0x1F8
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onDeselectHandlerObj; //0x20C
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onclickHandlerObj; //0x220
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *onClickHandler; //0x234
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *onSelectHandler; //0x238
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> *onDeselectHandler; //0x23C
    u32 buttonId; //0x240
    u32 playerCount; //0x244
    u8 unknown_0x248[0x254-0x248];
    
};//Total Size 0x254
class TabControl;
class TabControlOptionButton;
class TabControlChangeHandler{
public:
    PtmfHolder_3A<Screen, void, TabControlOptionButton*, u32, u32> onChangeHandler;
}; //total size 0x14

class TabControlOptionButton : public LayoutUIControl{
public:
    TabControlOptionButton(); //80638048
    virtual ~TabControlOptionButton(); //8063813c vtable 808beb80
    virtual void Init(); //80638370
    virtual void Update(); //8063851c
    virtual int GetStaticInstanceAddr(); //80638e30
    virtual char* getClassName(); //80638038
    void InitLayout(u32 buttonId, char *folderName, char *ctrName, char *variant, const char **anims, bool r8, bool inacessible); //806381a4
    void HandleSelect(u32 handlerId, u32 r5); //80638988
    void HandleDeselect(u32 handlerId, u32 r5); //80638a7c
    void HandleClick(u32 handlerId, u32 r5); //80638c64
    void ChangeButtonState(bool state); //true to select, false to unselect 80638830
    ControlActionHolder actionHolder; //0x174 to 0x1F8
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onSelectHandler; //80638988
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onDeselectHandler; //80638a7c //0x20c
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onclickHandler; //80638c64
    u32 buttonId; //0x234
    nw4r::lyt::Pane *fuchi_pattern;
    nw4r::lyt::Pane *color_base;
}; //total size 0x240

class TabControl : public LayoutUIControl { //change mii top buttons contains OptionButtons 808beb80
public:
    TabControl(); //80637ad0 
    virtual ~TabControl(); //80637b2c vtable 808beb3c
    virtual void Init(); //80637EC0
    virtual void Update(); //80637f94
    virtual int GetStaticInstanceAddr(); //80638e24
    virtual char* getClassName(); //80637ac4
    virtual void func_0x3C(); //80637d20
    virtual void func_0x40(); //80638e20
    void InitLayout(u32 buttonsCount,u32 initialButtonId, char *folderName, char *ctrName, const char **optionVariants, u32 localPlayerCount, bool r10, bool inacessible);  //80637ba4

    void SetOnChangeHandler(TabControlChangeHandler *handler); //80637d24
    //80637ba4 r10 stored 0x74 actionHolder, last param 0x75
    PtmfHolder_3A<Screen, void, TabControl*, u32, u32> *onChangeHandler; //r6 is buttonId, this is for the screen
    u32 localPlayerCount; //unsure //0x178
    u32 buttonsCount;
    u32 selectedButtonID; //0x180
    u32 unknown_0x184;
    TabControlOptionButton *optionButtonsArray; //0x188
}; //total size 0x188

class RadioButtonControl;
class UpDownControl;
class TextUpDownValueControl;
class TextUpDownValueControlTextControl;
class UpDownDisplayedText;

class RadioButtonChangeHandler{
public:
    PtmfHolder_3A<Screen, void, RadioButtonControl*, u32, u32> onChangeHandler; //0x0 vtable 808da3b0 80853714
}; //total size 0x14

class RadioButtonSelectHandler{
public:
    PtmfHolder_3A<Screen, void, RadioButtonControl*, u32, u32> onSelectHandler; //0x0 vtable 808da3b0 8085389c
}; //total size 0x14

class UpDownChangeHandler{
public:
    PtmfHolder_3A<Screen, void, UpDownControl*, u32, u32> onChangeHandler; //0x0 vtable 0x808da398 80853998
}; //total size 0x14

class UpDownClickHandler{
public:
    PtmfHolder_2A<Screen, void, UpDownControl*, u32> onClickHandler; //0x0 vtable 0x808da398 8085391c
}; //total size 0x14

class UpDownElementSelectHandler{
public:
    PtmfHolder_2A<Screen, void, UpDownControl*, u32> onElementSelectHandler; //0x0 vtable 0x808da398 80853988
}; //total size 0x14

class UpDownDisplayedTextScrollHandler{
public:
    PtmfHolder_2A<Screen, void, TextUpDownValueControlTextControl*, u32> onTextScrollHandler; //0x0 vtable 0x808da380 80853a10 r5 has buttonID
}; //total size 0x14

class RadioButtonControlOptionButton : public LayoutUIControl {
public:
    RadioButtonControlOptionButton(); //0x80606858
    virtual ~RadioButtonControlOptionButton(); //0x808baa24
    nw4r::lyt::Pane *fuchi_pattern;
    nw4r::lyt::Pane *color_base; //can't find it it seems
};//total size 0x17c

class RadioButtonControl : public LayoutUIControl {
public:
    RadioButtonControl(); //80605724
    virtual ~RadioButtonControl(); //806058c8 vtable 808ba9e0
    void Init(u8 localPlayerId); //80605d74
    void InitLayout(u32 buttonsCount, u32 initialButtonId, char *folderName, char *ctrName, char *variant, char *optionCtrName, const char **optionVariants, 
    u32 localPlayerCount, u8 arg10, u8 arg11);
    void SetOnChangeHandler(RadioButtonChangeHandler *handler);
    void SetOnSelectHandler(RadioButtonSelectHandler *handler);
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onSelectHandler; //0x174 vtable 808baa60 806062d8
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onDeselectHandler; //0x188 vtable 808baa60 80606434
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> unknown_0x19C; //0x19C vtable 808baa60 80606568
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onClickHandler; //0x1B0 vtable 808baa60 80606620
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onRightPressHandler; //0x1C4 vtable 808baa60 806066d4
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onLeftPressHandler; //0x1D8 vtable 808baa60 80606790
    u32 localPlayerCount; //0x1EC
    PtmfHolder_3A<InteractableScreen, void, RadioButtonControl*, u32, s32> *onChangeHandler; //0x1DF8
    u32 unknown_0x1F4; //init at 1
    u32 unknown_0x1F8;
    PtmfHolder_3A<InteractableScreen, void, RadioButtonControl*, u32, s32> *onSelectHandler2; //ptmfHolder
    u32 buttonsCount; //0x200
    u32 selectedButtonID; //init at -1
    u32 hoveredButtonID;
    u32 id; //if there are multiple radiobuttonctrol
    ControlActionHolder actionHolder; //0x210
    RadioButtonControlOptionButton *optionButtonsArray; //0x294
}; //total size 0x298

class UpDownControlUpDownButton : public LayoutUIControl {
public:
    UpDownControlUpDownButton(); //0x8063fdc0
    virtual ~UpDownControlUpDownButton(); //0x808bf4b0

    bool isVisibleOnIdle;
    u8 unknown_0x175[3]; //might be padding
    nw4r::lyt::Pane *fuchi_pattern;
    nw4r::lyt::Pane *color_base; //can't find it it seems
    nw4r::lyt::Pane *color_yajirushi; //can't find it either //0x180
    void *screenElementActionHolder_field0x34; //0x184
};//total size 0x188

class UpDownControl : public LayoutUIControl {
public:
    UpDownControl(); //0x8063e960
    virtual ~UpDownControl(); //0x8063eb5c vtable 808bf46c
    void SetOnChangeHandler(UpDownChangeHandler *handler);
    void SetOnClickHandler(UpDownClickHandler *handler);
    void SetOnElementSelectHandler(UpDownElementSelectHandler *handler);
    void InitLayout(u32 optionCount, u32 initialOptionId, char *folderName, char *ctrName, char *variant, char *buttonRCtrName, char *buttonRvariant, 
    char *buttonLCtrName, char *buttonLvariant, UpDownDisplayedText *text, u32 playerCount, u8 unk_13, bool isNotSelectable, bool isLooped, bool onClickSendsToNextControl);
    //unk_13 0
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onSelectHandler; //0x174 vtable 808bf4ec 8063f694
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onDeselectHandler; //0x188 vtable 808bf4ec 8063f7c4
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> unknown_0x19C; //0x19C vtable 808bf4ec 8063f8c8
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> unknown_0x1B0; //0x1B0 vtable 808bf4ec 8063f8e0
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onClickPressHandler; //0x1C4 vtable 808bf4ec 8063f8f8
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onRightPressHandler; //0x1D8 vtable 808bf4ec 8063fb64
    PtmfHolder_2A<LayoutUIControl, void, u32, u32> onLeftPressHandler; //0x1EC vtable 808bf4ec 8063fc90
    u32 localPlayerCount; //0x200
    u32 currentlySelectedOption; //0x204
    u32 buttonsCount; //0x208
    u32 id; //if there are multiple UpDownControl
    u8 unknown_0x210[4];
    UpDownDisplayedText *text; //0x214
    ControlActionHolder actionHolder; //0x218
    PtmfHolder_3A<InteractableScreen, void, UpDownControl*, u32, s32> *onChangeHandler;
    PtmfHolder_0A<InteractableScreen, void> *onClickHandler;
    PtmfHolder_0A<InteractableScreen, void> *onElementSelectHandler;
    u32 unknown_0x2A8;
    nw4r::lyt::Pane *fuchi_pattern;
    nw4r::lyt::Pane *color_base; //can't find it it seems
    UpDownControlUpDownButton arrowButtons[2];
    u32 soundID; //init at 0x13, sound to play when the arrows are pressed
};  //0x5C8


class TextUpDownValueControlTextControl : public LayoutUIControl {
    TextUpDownValueControlTextControl(); // 806410e0
    virtual ~TextUpDownValueControlTextControl(); //8064111c vtable 808bf430
}; //total size 0x174

class UpDownDisplayedText {
public:
    UpDownDisplayedText(){}; //inlined? //808bf3d8 vtable
    virtual PtmfHolder_2A<InteractableScreen, void, TextUpDownValueControlTextControl*, u32> *GetOnScrollHandler(); //just a blr as handler at 0x0 80642988, called at 8063eccc
    virtual void Load(u32 r4); //80640aa0
    virtual void OnSelectDeselect(bool state); //called with 0 to deselect, 1 to select, makes the box have the yellow animated border
    virtual void SetMsgIDOnLoad(u32 optionId);
    virtual void OnScroll(u32 optionId, u8 direction); //0 = left, 1 = right
    virtual void OnClick();
    virtual void Unk_0x20(); //calls animation stuff
    virtual void Unk_0x24(); //idk order is weird
    virtual void Unk_0x28(); //fuchi_pattern update? stores thinks in its material
    virtual void Unk_0x2C(); //color_base update?

    PtmfHolder_2A<InteractableScreen, void, TextUpDownValueControlTextControl*, u32> *onScrollSelectHandler; //when you scroll, r5 has button ID
    
}; //total size 0x2F0

class TextUpDownValueControl : public LayoutUIControl, public UpDownDisplayedText {
public:
    TextUpDownValueControl(); //806405d0
    virtual ~TextUpDownValueControl(); //8064063C vtable 808bf39c
    virtual void Init();
    virtual void Update();
    virtual int GetStaticInstanceAddr();
    virtual char *getClassName();
    void SetOnSelectHandler(UpDownDisplayedTextScrollHandler *handler);

    void InitLayout(char *folderName, char *ctrName, char *variant, char *textControlCtrName, char *textControlVariant); 
    TextUpDownValueControlTextControl textControl[2]; //array 0x17C
    TextUpDownValueControlTextControl *activeTextValueControl; // 0x464
    TextUpDownValueControlTextControl *otherTextValueControl; // 0x468 they invert everytime the button is moved, might mean each textControl only holds half?
    nw4r::lyt::Pane *fuchi_pattern;
    nw4r::lyt::Pane *color_base; //0x470
}; //total size 0x474

class CtrlMenuBackButton : public PushButton{
public:
    CtrlMenuBackButton(){
        bmgId = 0x7d1;
        unknown_0x258 = 0;
        alpha = 0.0f;
    }; //inlined vtable 808d374c
    virtual ~CtrlMenuBackButton(); //805be7f4
    u32 bmgId; //0x254
    u32 unknown_0x258;
    float alpha;
    char *pane;
}; //total size 0x264

class ButtonDriver : public PushButton{
public:
    ButtonDriver(); //807e2808
    virtual ~ButtonDriver(); //0x807e2844 vtable 808d3074
    u8 unknown_0x254;
    u8 unknown_0x255[3]; //might be padding
    float unknown_0x258;
    nw4r::lyt::Pane *black_base;
}; //total size 0x260