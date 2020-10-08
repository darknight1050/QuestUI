#include "BeatSaberUI.hpp"

#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Components/ScrollViewContent.hpp"
#include "CustomTypes/Components/QuestUIScrollView.hpp"
#include "CustomTypes/Components/KeyboardController.hpp"

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "HMUI/HoverHintController.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/TextPageScrollView.hpp"
#include "GlobalNamespace/BoolSettingsController.hpp"
#include "GlobalNamespace/ReleaseInfoViewController.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "System/Convert.hpp"

#include "customlogger.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace TMPro;
using namespace HMUI;
using namespace Polyglot;

namespace QuestUI::BeatSaberUI {
    
    MainFlowCoordinator* getMainFlowCoordinator() {
        static MainFlowCoordinator* mainFlowCoordinator = nullptr;
        if(!mainFlowCoordinator)
            mainFlowCoordinator = Object::FindObjectOfType<MainFlowCoordinator*>();
        return mainFlowCoordinator;
    }

    TMP_FontAsset* getMainTextFont() {
        static TMP_FontAsset* mainTextFont = nullptr;
        if(!mainTextFont)
            mainTextFont = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TMP_FontAsset*>(), [](TMP_FontAsset* x) { return to_utf8(csstrtostr(x->get_name())) == "Teko-Medium SDF No Glow"; });
        return mainTextFont;
    }

    Sprite* getEditIcon() {
        static Sprite* editIcon = nullptr;
        if(!editIcon)
            editIcon = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Image*>(), [](Image* x) { return x->get_sprite() && to_utf8(csstrtostr(x->get_sprite()->get_name())) == "EditIcon"; })->get_sprite();
        return editIcon;
    }
    
    ViewController* CreateViewController(System::Type* type) {
        static auto name = il2cpp_utils::createcsstr("QuestUIViewController", il2cpp_utils::StringType::Permanent);
        ViewController* viewController = (ViewController*)GameObject::New_ctor(name)->AddComponent(type);
        Object::DontDestroyOnLoad(viewController->get_gameObject());

        RectTransform* rectTransform = viewController->get_rectTransform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        return viewController;
    }
    
    FlowCoordinator* CreateFlowCoordinator(System::Type* type) {
        static auto name = il2cpp_utils::createcsstr("QuestUIFlowCoordinator", il2cpp_utils::StringType::Permanent);
        FlowCoordinator* flowCoordinator = (FlowCoordinator*)GameObject::New_ctor(name)->AddComponent(type);
        flowCoordinator->baseInputModule = getMainFlowCoordinator()->baseInputModule;
        return flowCoordinator;
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, anchoredPosition, UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static auto name = il2cpp_utils::createcsstr("QuestUIText", il2cpp_utils::StringType::Permanent);
        GameObject* gameObj = GameObject::New_ctor(name);
        gameObj->SetActive(false);

        TextMeshProUGUI* textMesh = gameObj->AddComponent<TextMeshProUGUI*>();
        RectTransform* rectTransform = textMesh->get_rectTransform();
        textMesh->set_font(getMainTextFont());
        rectTransform->SetParent(parent, false);
        textMesh->set_text(il2cpp_utils::createcsstr(text));
        textMesh->set_fontSize(4.0f);
        textMesh->set_color(UnityEngine::Color::get_white());

        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchoredPosition(anchoredPosition);
        rectTransform->set_sizeDelta(sizeDelta);

        gameObj->SetActive(true);
        return textMesh;
    }

    void SetButtonText(Button* button, std::string text) {
        LocalizedTextMeshProUGUI* localizer = button->GetComponentInChildren<LocalizedTextMeshProUGUI*>();
        if (localizer)
            Object::Destroy(localizer);
        TextMeshProUGUI* textMesh = button->GetComponentInChildren<TextMeshProUGUI*>();
        if(textMesh)
            textMesh->set_text(il2cpp_utils::createcsstr(text));
    }

    void SetButtonTextSize(Button* button, float fontSize) {
        TextMeshProUGUI* textMesh = button->GetComponentInChildren<TextMeshProUGUI*>();
        if(textMesh)
            textMesh->set_fontSize(fontSize);
    }

    void ToggleButtonWordWrapping(Button* button, bool enableWordWrapping) {
        TextMeshProUGUI* textMesh = button->GetComponentInChildren<TextMeshProUGUI*>();
        if(textMesh)
            textMesh->set_enableWordWrapping(enableWordWrapping);
    }

    void SetButtonIcon(Button* button, Sprite* icon) {
        auto* array = button->GetComponentsInChildren<Image*>();
        if(array->Length() > 1)
            ArrayUtil::First(array, [](Image* x) { return to_utf8(csstrtostr(x->get_name())) == "Icon";})->set_sprite(icon);
    }

    void SetButtonBackground(Button* button, Sprite* background) {
        auto* array = button->GetComponentsInChildren<Image*>();
        if(array->Length() > 0)
            array->values[0]->set_sprite(background);
    }

    Button* CreateUIButton(Transform* parent, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, UnityAction* onClick, std::string buttonText, Sprite* icon){
        Button* button = CreateUIButton(parent, buttonTemplate, anchoredPosition, onClick, buttonText, icon);
        ((RectTransform*)button->get_transform())->set_sizeDelta(sizeDelta);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityAction* onClick, std::string buttonText, Sprite* icon){
        Button* button = CreateUIButton(parent, buttonTemplate, onClick, buttonText, icon);
        ((RectTransform*)button->get_transform())->set_anchoredPosition(anchoredPosition);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonTemplate, UnityAction* onClick, std::string buttonText, Sprite* icon){
        Button* button = Object::Instantiate(ArrayUtil::Last(Resources::FindObjectsOfTypeAll<Button*>(), [&buttonTemplate](Button* x) { return to_utf8(csstrtostr(x->get_name())) == buttonTemplate; }), parent, false);
        button->set_onClick(Button::ButtonClickedEvent::New_ctor());
        if(onClick)
            button->get_onClick()->AddListener(onClick);
    
        static auto name = il2cpp_utils::createcsstr("QuestUIButton", il2cpp_utils::StringType::Permanent);
        button->set_name(name);

        RectTransform* rectTransform = (RectTransform*)button->get_transform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));

        SetButtonText(button, buttonText);
        if(icon)
            SetButtonIcon(button, icon);
        return button;
    }

    Image* CreateImage(Transform* parent, Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static auto name = il2cpp_utils::createcsstr("QuestUIImage", il2cpp_utils::StringType::Permanent);
        GameObject* gameObj = GameObject::New_ctor(name);
        Image* image = gameObj->AddComponent<Image*>();
        image->get_transform()->SetParent(parent, false);
        image->set_sprite(sprite);
        RectTransform* rectTransform = (RectTransform*)image->get_transform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchoredPosition(anchoredPosition);
        rectTransform->set_sizeDelta(sizeDelta);
        
        gameObj->AddComponent<LayoutElement*>();
        return image;
    }

    Sprite* Base64ToSprite(std::string base64, int width, int height)
    {
        Array<uint8_t>* bytes = System::Convert::FromBase64String(il2cpp_utils::createcsstr(base64));
        Texture2D* texture = Texture2D::New_ctor(width, height, TextureFormat::RGBA32, false, false);
        if(ImageConversion::LoadImage(texture, bytes, false))
            return Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)width, (float)height), UnityEngine::Vector2(0.5f,0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, UnityEngine::Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        return nullptr;
    }
    
    GridLayoutGroup* CreateGridLayoutGroup(Transform* parent) {
        static auto name = il2cpp_utils::createcsstr("QuestUIGridLayoutGroup", il2cpp_utils::StringType::Permanent);
        RectTransform* rectTransform = GameObject::New_ctor(name, typeof(GridLayoutGroup*), typeof(ContentSizeFitter*), typeof(Backgroundable*))->GetComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        return rectTransform->GetComponent<GridLayoutGroup*>();
    }
    
    HorizontalLayoutGroup* CreateHorizontalLayoutGroup(Transform* parent) {
        static auto name = il2cpp_utils::createcsstr("QuestUIHorizontalLayoutGroup", il2cpp_utils::StringType::Permanent);
        GameObject* gameObject = GameObject::New_ctor(name, typeof(HorizontalLayoutGroup*), typeof(Backgroundable*));
        
        ContentSizeFitter* contentSizeFitter = gameObject->AddComponent<ContentSizeFitter*>();
        contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        
        gameObject->AddComponent<LayoutElement*>();
        return rectTransform->GetComponent<HorizontalLayoutGroup*>();
    }
    
    VerticalLayoutGroup* CreateVerticalLayoutGroup(Transform* parent) {
        static auto name = il2cpp_utils::createcsstr("QuestUIVerticalLayoutGroup", il2cpp_utils::StringType::Permanent);
        GameObject* gameObject = GameObject::New_ctor(name, typeof(VerticalLayoutGroup*), typeof(Backgroundable*));
        
        ContentSizeFitter* contentSizeFitter = gameObject->AddComponent<ContentSizeFitter*>();
        contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);

        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        
        gameObject->AddComponent<LayoutElement*>();
        return rectTransform->GetComponent<VerticalLayoutGroup*>();
    }

    Toggle* CreateToggle(Transform* parent, std::string text, UnityAction_1<bool>* onToggle)
    {
        return CreateToggle(parent, text, UnityEngine::Vector2(0.0f, 0.0f), onToggle);
    }
    
    Toggle* CreateToggle(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityAction_1<bool>* onToggle)
    {
        GameplayModifierToggle* baseSetting = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<GameplayModifierToggle*>(), [](GameplayModifierToggle* x){ return to_utf8(csstrtostr(x->get_name())) == "InstaFail"; }), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUICheckboxSetting", il2cpp_utils::StringType::Permanent);
        baseSetting->set_name(name);

        GameObject* gameObject = baseSetting->get_gameObject();
        gameObject->SetActive(false);

        Object::Destroy(baseSetting);
        Object::Destroy(gameObject->get_transform()->GetChild(0)->get_gameObject());
        Object::Destroy(gameObject->GetComponent<HoverHint*>()); 
        
        Toggle* toggle = gameObject->GetComponent<Toggle*>();
        toggle->onValueChanged = Toggle::ToggleEvent::New_ctor();
        if(onToggle)
            toggle->onValueChanged->AddListener(onToggle);
        TextMeshProUGUI* textMesh = gameObject->GetComponentInChildren<TextMeshProUGUI*>();
        textMesh->set_fontSize(5); //Change some settings to conform more to the List Dropdown/IncDec settings controllers
        textMesh->SetText(il2cpp_utils::createcsstr(text));
        RectTransform* rectTransform = textMesh->get_rectTransform();
        rectTransform->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, 0.0f));
        rectTransform->set_anchoredPosition(anchoredPosition);
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));;

        LayoutElement* layout = gameObject->GetComponent<LayoutElement*>(); //If Beat Games decides to add one later down the road.
        if (layout == nullptr) layout = gameObject->AddComponent<LayoutElement*>(); //For the time being, they dont have one, so time to add one myself!
        layout->set_preferredWidth(90); //Again, to conform to List Dropdown/IncDec settings controllers
        layout->set_preferredHeight(8);

        gameObject->SetActive(true);
        return toggle;
    }

    GameObject* CreateLoadingIndicator(Transform* parent, UnityEngine::Vector2 anchoredPosition)
    {
        GameObject* loadingIndicator = GameObject::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<GameObject*>(), [](GameObject* x){ return to_utf8(csstrtostr(x->get_name())) == "LoadingIndicator"; }), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUILoadingIndicator", il2cpp_utils::StringType::Permanent);
        loadingIndicator->set_name(name);

        loadingIndicator->AddComponent<LayoutElement*>();

        RectTransform* rectTransform = loadingIndicator->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        return loadingIndicator;
    }

    HoverHint* AddHoverHint(GameObject* gameObject, std::string text){
        HoverHint* hoverHint = gameObject->AddComponent<HoverHint*>();
        hoverHint->set_text(il2cpp_utils::createcsstr(text));
        hoverHint->hoverHintController = ArrayUtil::First(Resources::FindObjectsOfTypeAll<HoverHintController*>());
        return hoverHint;
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, UnityAction_1<float>* onValueChange) {
        return CreateIncrementSetting(parent, UnityEngine::Vector2(0.0f, 0.0f), text, decimals, increment, currentValue, onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, UnityEngine::Vector2 anchoredPosition, std::string text, int decimals, float increment, float currentValue, UnityAction_1<float>* onValueChange){
        BoolSettingsController* baseSetting  = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<BoolSettingsController*>(), [](BoolSettingsController* x){ return to_utf8(csstrtostr(x->get_name())) == "Fullscreen";}), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUIIncDecSetting", il2cpp_utils::StringType::Permanent);
        baseSetting->set_name(name);
        
        GameObject* gameObject = baseSetting->get_gameObject();
        Object::Destroy(baseSetting);
        gameObject->SetActive(false);

        IncrementSetting* setting = gameObject->AddComponent<IncrementSetting*>();
        setting->Decimals = decimals;
        setting->Increment = increment;
        setting->CurrentValue = currentValue;
        setting->OnValueChange = onValueChange;
        Transform* child = gameObject->get_transform()->GetChild(1);
        setting->Text = child->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
        setting->Text->SetText(setting->GetRoundedString());
        Button* decButton = ArrayUtil::First(child->GetComponentsInChildren<Button*>());
        Button* incButton = ArrayUtil::Last(child->GetComponentsInChildren<Button*>());
        decButton->set_interactable(true);
        incButton->set_interactable(true);
        decButton->get_onClick()->AddListener(il2cpp_utils::MakeAction<UnityAction>(il2cpp_functions::class_get_type(classof(UnityAction*)), setting, +[](IncrementSetting* setting){ setting->DecButtonPressed(); }));
        incButton->get_onClick()->AddListener(il2cpp_utils::MakeAction<UnityAction>(il2cpp_functions::class_get_type(classof(UnityAction*)), setting, +[](IncrementSetting* setting){ setting->IncButtonPressed(); }));
        
        child->GetComponent<RectTransform*>()->set_sizeDelta(UnityEngine::Vector2(40, 0));
        TextMeshProUGUI* textMesh = gameObject->GetComponentInChildren<TextMeshProUGUI*>();
        textMesh->SetText(il2cpp_utils::createcsstr(text));
        gameObject->AddComponent<ExternalComponents*>()->Add(textMesh);

        Object::Destroy(textMesh->GetComponent<LocalizedTextMeshProUGUI*>());

        LayoutElement* layoutElement = gameObject->AddComponent<LayoutElement*>();
        layoutElement->set_preferredWidth(90);

        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        gameObject->SetActive(true);    

        return setting;
    }

    /*GameObject* CreateScrollView(Transform* parent) {
        ReleaseInfoViewController* releaseInfoViewController = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<ReleaseInfoViewController*>()), parent);
        releaseInfoViewController->get_gameObject()->SetActive(true);
        TextPageScrollView* textScrollView = releaseInfoViewController->textPageScrollView;
        static auto textScrollViewName = il2cpp_utils::createcsstr("QuestUIScrollView", il2cpp_utils::StringType::Permanent);
        textScrollView->set_name(textScrollViewName);
        Button* pageUpButton = textScrollView->pageUpButton;
        Button* pageDownButton = textScrollView->pageDownButton;
        VerticalScrollIndicator* verticalScrollIndicator = textScrollView->verticalScrollIndicator; 
        RectTransform* viewport = textScrollView->viewport;
        GameObject::Destroy(textScrollView->text->get_gameObject());
        GameObject* gameObject = textScrollView->get_gameObject();
        GameObject::Destroy(textScrollView);
        gameObject->SetActive(false);

        QuestUIScrollView* scrollView = gameObject->AddComponent<QuestUIScrollView*>();
        scrollView->pageUpButton = pageUpButton;
        scrollView->pageDownButton = pageDownButton;
        scrollView->verticalScrollIndicator = verticalScrollIndicator;
        scrollView->viewport = viewport;

        viewport->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        viewport->set_anchorMin(UnityEngine::Vector2(1.0f, 1.0f));
        scrollView->SetReserveButtonSpace(false);

        static auto parentObjName = il2cpp_utils::createcsstr("QuestUIScrollViewContent", il2cpp_utils::StringType::Permanent);
        GameObject* parentObj = GameObject::New_ctor(parentObjName);
        parentObj->get_transform()->SetParent(viewport, false);

        VerticalLayoutGroup* verticalLayout = parentObj->AddComponent<VerticalLayoutGroup*>();
        verticalLayout->set_childForceExpandHeight(false);
        verticalLayout->set_childForceExpandWidth(false);

        RectTransform* rectTransform = parentObj->GetComponent<RectTransform*>();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, -1.0f));
        
        parentObj->AddComponent<ScrollViewContent*>()->scrollView = scrollView;

        static auto childName = il2cpp_utils::createcsstr("QuestUIScrollViewContentContainer", il2cpp_utils::StringType::Permanent);
        GameObject* child = GameObject::New_ctor(childName);
        child->get_transform()->SetParent(rectTransform, false);

        VerticalLayoutGroup* layoutGroup = child->AddComponent<VerticalLayoutGroup*>();
        layoutGroup->set_childControlHeight(false);
        layoutGroup->set_childForceExpandHeight(false);
        layoutGroup->set_childAlignment(TextAnchor::MiddleCenter);
        layoutGroup->set_spacing(0.5f);

        child->AddComponent<ContentSizeFitter*>();
        child->AddComponent<LayoutElement*>();

        ExternalComponents* externalComponents = child->AddComponent<ExternalComponents*>();
        externalComponents->Add(scrollView);
        externalComponents->Add(scrollView->get_transform());
        externalComponents->Add(gameObject->AddComponent<LayoutElement*>());

        scrollView->contentRectTransform = rectTransform;

        gameObject->SetActive(true);
        return child;
    }

    GameObject* CreateScrollableSettingsContainer(Transform* parent) {
        GameObject* content = CreateScrollView(parent);
        ExternalComponents* externalComponents = content->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(2, 6));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(0, 20));
        static auto name = il2cpp_utils::createcsstr("QuestUIScrollableSettingsContainer", il2cpp_utils::StringType::Permanent);
        scrollTransform->get_gameObject()->set_name(name);
        externalComponents->Get<QuestUIScrollView*>()->SetReserveButtonSpace(true);
        return content;
    }*/

    GameObject* CreateModalView(Transform* parent) {
        static auto name = il2cpp_utils::createcsstr("QuestUIModalView", il2cpp_utils::StringType::Permanent);
        GameObject* gameObject = GameObject::New_ctor(name);
        gameObject->SetActive(false);

        RectTransform* rectTransform = gameObject->AddComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

        ModalView* modalView = gameObject->AddComponent<ModalView*>();
        ModalView* yoinkFromView = ArrayUtil::First(Resources::FindObjectsOfTypeAll<ModalView*>(), [](ModalView* x){ return to_utf8(csstrtostr(x->get_name())) == "TableView";});
        modalView->presentPanelAnimations = yoinkFromView->presentPanelAnimations;
        modalView->dismissPanelAnimation = yoinkFromView->dismissPanelAnimation;

        GameObject* child = GameObject::New_ctor();
        child->get_transform()->SetParent(rectTransform, false);
        RectTransform* shadowTransform = child->get_gameObject()->AddComponent<RectTransform*>();
        shadowTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        shadowTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        shadowTransform->set_sizeDelta(UnityEngine::Vector2(10.0f, 10.0f));
        child->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel-shadow"));

        child = GameObject::New_ctor();
        child->get_transform()->SetParent(rectTransform, false);
        RectTransform* backgroundTransform = child->get_gameObject()->AddComponent<RectTransform*>();
        backgroundTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        backgroundTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        backgroundTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

        Backgroundable* backgroundable = child->get_gameObject()->AddComponent<Backgroundable*>();
        backgroundable->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
        auto background = backgroundable->background;
        background->set_color(UnityEngine::Color(0.706f, 0.706f, 0.706f, 1.0f));
        background->set_material(ArrayUtil::First(Resources::FindObjectsOfTypeAll<Material*>(), [](Material* x){ return to_utf8(csstrtostr(x->get_name())) == "UIFogBG"; }));

        ExternalComponents* externalComponents = child->AddComponent<ExternalComponents*>();
        externalComponents->Add(modalView);
        externalComponents->Add(rectTransform);

        return child;
    }

    GameObject* CreateKeyboard(Transform* parent) {
        GameObject* gameObject = CreateModalView(parent);

        ExternalComponents* externalComponents = gameObject->GetComponent<ExternalComponents*>();

        RectTransform* windowTransform = externalComponents->Get<RectTransform*>();
        static auto name = il2cpp_utils::createcsstr("QuestUIModalKeyboard", il2cpp_utils::StringType::Permanent);
        windowTransform->set_name(name);
        windowTransform->set_sizeDelta(UnityEngine::Vector2(135.0f, 75.0f));

        static auto parentName = il2cpp_utils::createcsstr("KeyboardParent", il2cpp_utils::StringType::Permanent);
        RectTransform* parentTransform = GameObject::New_ctor(parentName)->AddComponent<RectTransform*>();
        parentTransform->SetParent(gameObject->get_transform(), false);

        parentTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 12.0f));
        externalComponents->Add(parentTransform->get_gameObject()->AddComponent<KeyboardController*>());
        return gameObject;
    }

    StringSetting* CreateStringSetting(Transform* parent, std::string settingsName, std::string currentValue, UnityAction_1<Il2CppString*>* onValueChange) {
        return CreateStringSetting(parent, UnityEngine::Vector2(0.0f, 0.0f), settingsName, currentValue, onValueChange);
    }

    StringSetting* CreateStringSetting(Transform* parent, UnityEngine::Vector2 anchoredPosition, std::string settingsName, std::string currentValue, UnityAction_1<Il2CppString*>* onValueChange) {
        BoolSettingsController* baseSetting = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<BoolSettingsController*>(), [](BoolSettingsController* x){ return to_utf8(csstrtostr(x->get_name())) == "Fullscreen"; }), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUIStringSetting", il2cpp_utils::StringType::Permanent);
        baseSetting->set_name(name);
        
        GameObject* gameObject = baseSetting->get_gameObject();
        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);
        Object::Destroy(baseSetting);
        gameObject->SetActive(false);

        static auto valuePickerName = il2cpp_utils::createcsstr("ValuePicker", il2cpp_utils::StringType::Permanent);
        Transform* valuePick = rectTransform->Find(valuePickerName);
        Button* decButton = ArrayUtil::First(valuePick->GetComponentsInChildren<Button*>());
        decButton->set_enabled(false);
        decButton->set_interactable(true);
        static auto arrowName = il2cpp_utils::createcsstr("Arrow", il2cpp_utils::StringType::Permanent);
        GameObject::Destroy(decButton->get_transform()->Find(arrowName)->get_gameObject());

        StringSetting* setting = gameObject->AddComponent<StringSetting*>();
        setting->Text = ArrayUtil::First(valuePick->GetComponentsInChildren<TextMeshProUGUI*>());
        setting->Text->set_alignment(TextAlignmentOptions::MidlineRight);
        setting->Text->set_enableWordWrapping(false);
        setting->CurrentValue = il2cpp_utils::createcsstr(currentValue);
        setting->Text->set_text(setting->CurrentValue);
        setting->OnValueChange = onValueChange;
        Button* editButton = ArrayUtil::Last(valuePick->GetComponentsInChildren<Button*>());
        RectTransform* boundingBox = (RectTransform*)valuePick;
            
        TextMeshProUGUI* settingsNameMesh = gameObject->GetComponentInChildren<TextMeshProUGUI*>();
        settingsNameMesh->set_text(il2cpp_utils::createcsstr(settingsName));
        gameObject->AddComponent<ExternalComponents*>()->Add(settingsNameMesh);
        GameObject::Destroy(settingsNameMesh->GetComponent<LocalizedTextMeshProUGUI*>());
            
        gameObject->GetComponent<LayoutElement*>()->set_preferredWidth(90);

        Image* icon = editButton->get_transform()->Find(arrowName)->GetComponent<Image*>();
        icon->set_name(il2cpp_utils::createcsstr("EditIcon"));
        icon->set_sprite(getEditIcon());
        icon->GetComponent<RectTransform*>()->set_sizeDelta(UnityEngine::Vector2(4.0f, 4.0f));
        editButton->set_interactable(true);
        editButton->GetComponent<RectTransform*>()->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));

        ExternalComponents* externalComponents = CreateKeyboard(rectTransform)->GetComponent<ExternalComponents*>();
        KeyboardController* keyboardController = externalComponents->Get<KeyboardController*>();
        setting->KeyboardController = keyboardController;
        setting->ModalView = externalComponents->Get<ModalView*>();
        keyboardController->add_confirmPressed(il2cpp_utils::MakeAction<System::Action_1<Il2CppString*>>(il2cpp_functions::class_get_type(classof(System::Action_1<Il2CppString*>*)), setting, +[](StringSetting* setting, Il2CppString* text) { setting->ConfirmPressed(text); }));
        keyboardController->add_cancelPressed(il2cpp_utils::MakeAction<System::Action>(il2cpp_functions::class_get_type(classof(System::Action*)), setting, +[](StringSetting* setting) { setting->CancelPressed(); }));
        editButton->get_onClick()->AddListener(il2cpp_utils::MakeAction<UnityAction>(il2cpp_functions::class_get_type(classof(UnityAction*)), setting, +[](StringSetting* setting) { setting->ButtonPressed(); }));
        gameObject->SetActive(true);
        return setting;
    }

}
