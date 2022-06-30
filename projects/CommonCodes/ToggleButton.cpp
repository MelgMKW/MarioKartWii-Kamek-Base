#include <Kamek/kamek.hpp>
#include <ToggleButton.hpp>

ToggleButton::ToggleButton(){
    this->localPlayerCount = 1;
    this->onSelectHandler.subject = this;
    this->onSelectHandler.ptmf = static_cast<void(LayoutUIControl::*)(u32, u32)>(&ToggleButton::HandleSelect);
    this->onDeselectHandler.subject = this;
    this->onDeselectHandler.ptmf = static_cast<void(LayoutUIControl::*)(u32, u32)>(&ToggleButton::HandleDeselect);
    this->onClickHandler.subject = this;
    this->onClickHandler.ptmf = static_cast<void(LayoutUIControl::*)(u32, u32)>(&ToggleButton::HandleClick);
    this->state = false;
};

void ToggleButton::Init(){
    LayoutUIControl::Init();
    PaneGroup *group = this->animator.GetPaneGroupById(0); //loop
    group->PlayAndDisableAnimation(0, 0.0f);
    
    group = this->animator.GetPaneGroupById(1); //select, the looping animation
    group->PlayAnimation(0, 0.0f);

    group = this->animator.GetPaneGroupById(2); //selectin, the button getting bigger on select
    group->PlayAnimation(1, 0.0f);

    group = this->animator.GetPaneGroupById(3); //ok, text animation on click
    group->PlayAnimation(1, 0.0f);

    group = this->animator.GetPaneGroupById(4); //choice, pattern and colours on click
    group->PlayAnimation(0, 0.0f);
    this->actionHolder.SetLocalPlayerCount(this->localPlayerCount);
};

void ToggleButton::Update(){
    LayoutUIControl::Update();
    PaneGroup *group = this->animator.GetPaneGroupById(1);
    if (group->currentAnimation == 0){
        group = this->animator.GetPaneGroupById(0);
        group->PlayAndDisableAnimation(0, 30.0f);
        this->drawPriority = 0.0f;
    }
    else{
        group = this->animator.GetPaneGroupById(0);
        nw4r::lyt::AnimTransformBasic *transform = group->groupAnims[group->currentAnimation].transform;
        if (transform != NULL){
            group->isActive = true;
            this->drawPriority = 0.0f;
        }   
    }
    if (this->fuchi_pattern != NULL){
        ControlActionHolder *holder = &this->actionHolder;
        RGBA16 firstHalf;
        RGBA16 secondHalf;
        holder->SetBorderColours(&firstHalf, &secondHalf);
        nw4r::lyt::Material *material = this->fuchi_pattern->GetMaterial();
        material->colours2.red = firstHalf.red;
        material->colours2.green = firstHalf.green;
        material->colours2.blue = firstHalf.blue;
        material->colours2.alpha = firstHalf.alpha;

        material->colours3.red = secondHalf.red;
        material->colours3.green = secondHalf.green;
        material->colours3.blue = secondHalf.blue;
        material->colours3.alpha = secondHalf.alpha;
    }
}

void ToggleButton::InitLayout(char *folderName, char *ctrName, char *variant, u32 localPlayerCount, bool r10, bool inacessible){
    this->localPlayerCount = localPlayerCount;
    ControlLoader loader(this);
    const char *anims[24] = {"Loop", "Loop", NULL, 
    "Select", "Free", "FreeToSelect", "Select", "SelectToFree", NULL, 
    "SelectIn","SelectIn", "SelectStop", NULL, 
    "OK", "OK", "OKStop", NULL, 
    "Choice", "ChoiceOff", "ChoiceOffToOn", "ChoiceOn", "ChoiceOnToOff", NULL, NULL};

    loader.Load(folderName, ctrName, variant, anims);
    nw4r::lyt::Pane *pane = this->layout.GetPaneByName("fuchi_pattern");
    this->fuchi_pattern = pane;

    pane = this->layout.GetPaneByName("touch");
    this->actionHolder.selectHandler.pane = pane;
    this->actionHolder.selectHandler.onSelectHandler = (&this->onSelectHandler);
    this->actionHolder.selectHandler.onDeselectHandler = (&this->onDeselectHandler);
    this->actionHolder.Init(0, false, false);
    this->actionHolder.SetHandler(FORWARD_PRESS, (ptmfHolder*)(&this->onClickHandler), false);

    ScreenActionController *controller = this->parentGroup->parentScreen->screenActionController;
    controller->AddActionHolder(&this->actionHolder);
}

void ToggleButton::SetOnChangeHandler(ToggleButtonChangeHandler *handler){
    this->onChangeHandler = &handler->onChangeHandler;
}

void ToggleButton::ToggleState(bool state){
    PaneGroup *group = this->animator.GetPaneGroupById(4);
    if (this->state != state){
        if(this->state == true) group->PlayAnimation(0, 0.0f);
        else group->PlayAnimation(2, 0.0f);
        this->state = !this->state;
        u32 bmgId = 0x2400;
        if(state) bmgId = 0x2401;
        this->SetMsgId(bmgId, 0);
    }
}

void ToggleButton::HandleSelect(u32 handlerId, u32 r5){
    PaneGroup *group = this->animator.GetPaneGroupById(1);
    if (group->currentAnimation == 0){
        group->PlayAnimation(1, 0.0f);
    }
    else if (group->currentAnimation == 3){
        float frameSize = (float) group->groupAnims[group->currentAnimation].transform->GetFrameSize();
        group->PlayAnimationAt(1, 1.0f-4.0f/frameSize);
    }
    group = this->animator.GetPaneGroupById(2);
    group->PlayAnimation(0, 0.0f);
    this->PlaySound(0xC, handlerId);
}

void ToggleButton::HandleDeselect(u32 handlerId, u32 r5){
    PaneGroup *group = this->animator.GetPaneGroupById(1);
    if (group->currentAnimation == 1){
        float frameSize = (float) group->groupAnims[group->currentAnimation].transform->GetFrameSize();
        group->PlayAnimationAt(1, 1.0f-38.0f/frameSize);
    }
    else if (group->currentAnimation == 2) group->PlayAnimation(3, 0.0f);
}

void ToggleButton::HandleClick(u32 handlerId, u32 r5){
    PaneGroup *group = this->animator.GetPaneGroupById(2);
    group->PlayAnimation(0, 0.0f);
    group = this->animator.GetPaneGroupById(3);
    group->PlayAnimation(0, 0.0f);
    
    group = this->animator.GetPaneGroupById(4); //choice group, off, offtoon, on, ontooff
    if (this->state == true && group->currentAnimation == 1 || this->state == false && group->currentAnimation == 3){
        float frameSize = (float) group->groupAnims[group->currentAnimation].transform->GetFrameSize();
        group->PlayAnimationAt(1, 1.0f-75.0f/frameSize);
    }
    else if(this->state == true && group->currentAnimation == 2) group->PlayAnimation(3, 0.0f);
    
    else if(this->state == false && group->currentAnimation == 0) group->PlayAnimation(1, 0.0f); 

    this->state = !this->state;
    this->PlaySound(0xD, -1);
    this->onChangeHandler->operator()(this);
    u32 bmgId = 0x2400;
    if(state) bmgId = 0x2401;
    this->SetMsgId(bmgId, 0);
}
