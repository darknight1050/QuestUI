#include "BeatSaberUI.hpp"
#include "InternalBeatSaberUI.hpp"

#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
//#include "CustomTypes/Components/ScrollViewContent.hpp"
//#include "CustomTypes/Components/QuestUIScrollView.hpp"

#include "GlobalNamespace/UIKeyboardManager.hpp"
#include "GlobalNamespace/BoolSettingsController.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "GlobalNamespace/ReleaseInfoViewController.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/CanvasGroup.hpp"
#include "UnityEngine/AdditionalCanvasShaderChannels.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/UI/ScrollRect.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/HoverHintController.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableViewScroller.hpp"
#include "HMUI/TextPageScrollView.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "HMUI/InputFieldView_InputFieldChanged.hpp"
#include "HMUI/UIKeyboard.hpp"
#include "VRUIControls/VRGraphicRaycaster.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "System/Convert.hpp"
#include "Zenject/DiContainer.hpp"

#include "customlogger.hpp"

#define DEFAULT_BUTTONTEMPLATE "PracticeButton"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace TMPro;
using namespace HMUI;
using namespace Polyglot;
using namespace VRUIControls;
using namespace Zenject;

namespace QuestUI::BeatSaberUI {
    
    MainFlowCoordinator* mainFlowCoordinator = nullptr;
    MainFlowCoordinator* getMainFlowCoordinator() {
        if(!mainFlowCoordinator)
            mainFlowCoordinator = Object::FindObjectOfType<MainFlowCoordinator*>();
        return mainFlowCoordinator;
    }

    TMP_FontAsset* mainTextFont = nullptr;
    TMP_FontAsset* getMainTextFont() {
        if(!mainTextFont)
            mainTextFont = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TMP_FontAsset*>(), [](TMP_FontAsset* x) { return to_utf8(csstrtostr(x->get_name())) == "Teko-Medium SDF No Glow"; });
        return mainTextFont;
    }

    Sprite* editIcon = nullptr;
    Sprite* getEditIcon() {
        if(!editIcon)
            editIcon = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Image*>(), [](Image* x) { return x->get_sprite() && to_utf8(csstrtostr(x->get_sprite()->get_name())) == "EditIcon"; })->get_sprite();
        return editIcon;
    }

    PhysicsRaycasterWithCache* physicsRaycaster = nullptr;
    PhysicsRaycasterWithCache* GetPhysicsRaycasterWithCache()
    {
        if(!physicsRaycaster)
            physicsRaycaster = ArrayUtil::First(Resources::FindObjectsOfTypeAll<MainMenuViewController*>())->GetComponent<VRGraphicRaycaster*>()->physicsRaycaster;
        return physicsRaycaster;
    }

    DiContainer* diContainer = nullptr;
    DiContainer* GetDiContainer()
    {
        if(!diContainer)
            diContainer = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TextSegmentedControl*>(), [](TextSegmentedControl* x) { return to_utf8(csstrtostr(x->get_transform()->get_parent()->get_name())) == "PlayerStatisticsViewController" && x->container; })->container;
        return diContainer;
    }

    void clearCache() {
        mainFlowCoordinator = nullptr;
        mainTextFont = nullptr;
        editIcon = nullptr;
        physicsRaycaster = nullptr;
        diContainer = nullptr;
    }

    ViewController* CreateViewController(System::Type* type) {
        static auto name = il2cpp_utils::createcsstr("QuestUIViewController", il2cpp_utils::StringType::Manual);
        GameObject* go = GameObject::New_ctor(name);

        Canvas* cv = go->AddComponent<Canvas*>();
        Canvas* cvCopy = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Canvas*>(), [](Canvas* x) { return to_utf8(csstrtostr(x->get_name())) == "DropdownTableView";});
        cv->set_additionalShaderChannels(cvCopy->get_additionalShaderChannels());
        cv->set_overrideSorting(cvCopy->get_overrideSorting());
        cv->set_pixelPerfect(cvCopy->get_pixelPerfect());
        cv->set_referencePixelsPerUnit(cvCopy->get_referencePixelsPerUnit());
        cv->set_renderMode(cvCopy->get_renderMode());
        cv->set_scaleFactor(cvCopy->get_scaleFactor());
        cv->set_sortingLayerID(cvCopy->get_sortingLayerID());
        cv->set_sortingOrder(cvCopy->get_sortingOrder());
        cv->set_worldCamera(cvCopy->get_worldCamera());

        go->AddComponent<VRGraphicRaycaster*>()->physicsRaycaster = GetPhysicsRaycasterWithCache();
        go->AddComponent<CanvasGroup*>();
        auto vc = go->AddComponent(type);

        RectTransform* rectTransform = go->GetComponent<RectTransform*>();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        go->SetActive(false);
        return go->GetComponent<ViewController*>();
    }
    
    FlowCoordinator* CreateFlowCoordinator(System::Type* type) {
        static auto name = il2cpp_utils::createcsstr("QuestUIFlowCoordinator", il2cpp_utils::StringType::Manual);
        FlowCoordinator* flowCoordinator = (FlowCoordinator*)GameObject::New_ctor(name)->AddComponent(type);
        flowCoordinator->baseInputModule = getMainFlowCoordinator()->baseInputModule;
        return flowCoordinator;
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, anchoredPosition, UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static auto name = il2cpp_utils::createcsstr("QuestUIText", il2cpp_utils::StringType::Manual);
        GameObject* gameObj = GameObject::New_ctor(name);
        gameObj->SetActive(false);

        CurvedTextMeshPro* textMesh = gameObj->AddComponent<CurvedTextMeshPro*>();
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
        if(!icon) return;
        auto* array = button->GetComponentsInChildren<Image*>();
        if(array->Length() > 1)
            ArrayUtil::First(array, [](Image* x) { return to_utf8(csstrtostr(x->get_name())) == "Icon";})->set_sprite(icon);
    }

    void SetButtonBackground(Button* button, Sprite* background) {
        auto* array = button->GetComponentsInChildren<Image*>();
        if(array->Length() > 0)
            array->values[0]->set_sprite(background);
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, std::string buttonTemplate, UnityAction* onClick) {
        Button* button = Object::Instantiate(ArrayUtil::Last(Resources::FindObjectsOfTypeAll<Button*>(), [&buttonTemplate](Button* x) { return to_utf8(csstrtostr(x->get_name())) == buttonTemplate; }), parent, false);
        button->set_onClick(Button::ButtonClickedEvent::New_ctor());
        static auto name = il2cpp_utils::createcsstr("QuestUIButton", il2cpp_utils::StringType::Manual);
        button->set_name(name);
        if(onClick)
            button->get_onClick()->AddListener(onClick);

        Polyglot::LocalizedTextMeshProUGUI* localizer = button->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>();
        if (localizer != nullptr)
            GameObject::Destroy(localizer);
        ExternalComponents* externalComponents = button->get_gameObject()->AddComponent<ExternalComponents*>();

        TextMeshProUGUI* textMesh = button->GetComponentInChildren<TextMeshProUGUI*>();
        textMesh->set_richText(true);
        externalComponents->Add(textMesh);

        RectTransform* rectTransform = (RectTransform*)button->get_transform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));

        SetButtonText(button, buttonText);

        HorizontalLayoutGroup* horiztonalLayoutGroup = button->GetComponentInChildren<HorizontalLayoutGroup*>();
        if (horiztonalLayoutGroup != nullptr)
            externalComponents->Add(horiztonalLayoutGroup);
            
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityAction* onClick) {
        Button* button = CreateUIButton(parent, buttonText, buttonTemplate, onClick);
        button->GetComponent<RectTransform*>()->set_anchoredPosition(anchoredPosition);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, UnityAction* onClick) {
        Button* button = CreateUIButton(parent, buttonText, buttonTemplate, anchoredPosition, onClick);
        button->GetComponent<RectTransform*>()->set_sizeDelta(sizeDelta);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, UnityAction* onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, onClick);
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, UnityEngine::Vector2 anchoredPosition, UnityAction* onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, anchoredPosition, onClick);
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, UnityAction* onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, anchoredPosition, sizeDelta, onClick);
    }

    Image* CreateImage(Transform* parent, Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static auto name = il2cpp_utils::createcsstr("QuestUIImage", il2cpp_utils::StringType::Manual);
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
        static auto name = il2cpp_utils::createcsstr("QuestUIGridLayoutGroup", il2cpp_utils::StringType::Manual);
        RectTransform* rectTransform = GameObject::New_ctor(name, typeof(GridLayoutGroup*), typeof(ContentSizeFitter*), typeof(Backgroundable*))->GetComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        return rectTransform->GetComponent<GridLayoutGroup*>();
    }
    
    HorizontalLayoutGroup* CreateHorizontalLayoutGroup(Transform* parent) {
        static auto name = il2cpp_utils::createcsstr("QuestUIHorizontalLayoutGroup", il2cpp_utils::StringType::Manual);
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
        static auto name = il2cpp_utils::createcsstr("QuestUIVerticalLayoutGroup", il2cpp_utils::StringType::Manual);
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
        GameObject* gameObject = Object::Instantiate(ArrayUtil::First(ArrayUtil::Select<GameObject*>(Resources::FindObjectsOfTypeAll<Toggle*>(), [](Toggle* x){ return x->get_transform()->get_parent()->get_gameObject(); }), [](GameObject* x){ return to_utf8(csstrtostr(x->get_name())) == "Fullscreen";}), parent, false);
        GameObject* nameText = gameObject->get_transform()->Find(il2cpp_utils::createcsstr("NameText"))->get_gameObject();
        Object::Destroy(gameObject->GetComponent<BoolSettingsController*>());

        static auto name = il2cpp_utils::createcsstr("QuestUICheckboxSetting", il2cpp_utils::StringType::Manual);
        gameObject->set_name(name);

        gameObject->SetActive(false);

        Object::Destroy(nameText->GetComponent<LocalizedTextMeshProUGUI*>());
        
        Toggle* toggle = gameObject->GetComponentInChildren<Toggle*>();
        toggle->set_interactable(true);
        toggle->set_isOn(false);
        toggle->onValueChanged = Toggle::ToggleEvent::New_ctor();
        if(onToggle)
            toggle->onValueChanged->AddListener(onToggle);
        TextMeshProUGUI* textMesh = nameText->GetComponent<TextMeshProUGUI*>();
        textMesh->SetText(il2cpp_utils::createcsstr(text));
        textMesh->set_richText(true);
        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        LayoutElement* layout = gameObject->GetComponent<LayoutElement*>();
        layout->set_preferredWidth(90);
        gameObject->SetActive(true);
        return toggle;
    }

    /*GameObject* CreateLoadingIndicator(Transform* parent, UnityEngine::Vector2 anchoredPosition)
    {
        GameObject* loadingIndicator = GameObject::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<GameObject*>(), [](GameObject* x){ return to_utf8(csstrtostr(x->get_name())) == "LoadingIndicator"; }), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUILoadingIndicator", il2cpp_utils::StringType::Manual);
        loadingIndicator->set_name(name);

        loadingIndicator->AddComponent<LayoutElement*>();

        RectTransform* rectTransform = loadingIndicator->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        return loadingIndicator;
    }*/

    HoverHint* AddHoverHint(GameObject* gameObject, std::string text){
        HoverHint* hoverHint = gameObject->AddComponent<HoverHint*>();
        hoverHint->set_text(il2cpp_utils::createcsstr(text));
        hoverHint->hoverHintController = ArrayUtil::First(Resources::FindObjectsOfTypeAll<HoverHintController*>());
        return hoverHint;
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, UnityAction_1<float>* onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, UnityEngine::Vector2 anchoredPosition, UnityAction_1<float>* onValueChange){
        FormattedFloatListSettingsValueController* baseSetting = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<FormattedFloatListSettingsValueController*>(), [](FormattedFloatListSettingsValueController* x){ return to_utf8(csstrtostr(x->get_name())) == "VRRenderingScale"; }), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUIIncDecSetting", il2cpp_utils::StringType::Manual);
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
        setting->Text->set_richText(true);
        Button* decButton = ArrayUtil::First(child->GetComponentsInChildren<Button*>());
        Button* incButton = ArrayUtil::Last(child->GetComponentsInChildren<Button*>());
        decButton->set_interactable(true);
        incButton->set_interactable(true);
        decButton->get_onClick()->AddListener(il2cpp_utils::MakeAction<UnityAction>(il2cpp_functions::class_get_type(classof(UnityAction*)), setting, +[](IncrementSetting* setting){ setting->DecButtonPressed(); }));
        incButton->get_onClick()->AddListener(il2cpp_utils::MakeAction<UnityAction>(il2cpp_functions::class_get_type(classof(UnityAction*)), setting, +[](IncrementSetting* setting){ setting->IncButtonPressed(); }));
        
        child->GetComponent<RectTransform*>()->set_sizeDelta(UnityEngine::Vector2(40, 0));
        TextMeshProUGUI* textMesh = gameObject->GetComponentInChildren<TextMeshProUGUI*>();
        textMesh->SetText(il2cpp_utils::createcsstr(text));
        textMesh->set_richText(true);
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
        static auto textScrollViewName = il2cpp_utils::createcsstr("QuestUIScrollView", il2cpp_utils::StringType::Manual);
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

        static auto parentObjName = il2cpp_utils::createcsstr("QuestUIScrollViewContent", il2cpp_utils::StringType::Manual);
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

        static auto childName = il2cpp_utils::createcsstr("QuestUIScrollViewContentContainer", il2cpp_utils::StringType::Manual);
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
        static auto name = il2cpp_utils::createcsstr("QuestUIScrollableSettingsContainer", il2cpp_utils::StringType::Manual);
        scrollTransform->get_gameObject()->set_name(name);
        externalComponents->Get<QuestUIScrollView*>()->SetReserveButtonSpace(true);
        return content;
    }*/

    GameObject* CreateModalView(Transform* parent) {
        static auto name = il2cpp_utils::createcsstr("QuestUIModalView", il2cpp_utils::StringType::Manual);
        ModalView* yoinkFromView = ArrayUtil::First(Resources::FindObjectsOfTypeAll<ModalView*>(), [](ModalView* x){ return to_utf8(csstrtostr(x->get_name())) == "DropdownTableView"; });
        ModalView* modalView = Object::Instantiate(yoinkFromView, parent);
        modalView->presentPanelAnimations = yoinkFromView->presentPanelAnimations;
        modalView->dismissPanelAnimation = yoinkFromView->dismissPanelAnimation;
        modalView->container = GetDiContainer();
        modalView->GetComponent<VRGraphicRaycaster*>()->physicsRaycaster = GetPhysicsRaycasterWithCache();

        GameObject::Destroy(modalView->GetComponent<TableView*>());
        GameObject::Destroy(modalView->GetComponent<TableViewScroller*>());
        GameObject::Destroy(modalView->GetComponent<ScrollRect*>());

        for(int i = 0;i<modalView->get_transform()->get_childCount();i++) {
            RectTransform* child = (RectTransform*)modalView->get_transform()->GetChild(i);
            if(to_utf8(csstrtostr(child->get_name())) == "BG") {
                child->set_anchoredPosition(UnityEngine::Vector2::get_zero());
                child->set_sizeDelta(UnityEngine::Vector2::get_zero());
            }
            else
            {
                GameObject::Destroy(child->get_gameObject());
            }
        }
        RectTransform* rectTransform = modalView->GetComponent<RectTransform*>();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2::get_zero());
        GameObject* gameObject = modalView->get_gameObject();
        gameObject->set_name(name);
        gameObject->AddComponent<ExternalComponents*>()->Add(modalView);
        return gameObject;
    }

    InputFieldView* CreateStringSetting(Transform* parent, std::string settingsName, std::string currentValue, UnityAction_1<Il2CppString*>* onValueChange) {
        return CreateStringSetting(parent, settingsName, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    InputFieldView* CreateStringSetting(Transform* parent, std::string settingsName, std::string currentValue, UnityEngine::Vector2 anchoredPosition, UnityAction_1<Il2CppString*>* onValueChange) {
        static auto name = il2cpp_utils::createcsstr("QuestUIStringSetting", il2cpp_utils::StringType::Manual);
        InputFieldView* originalfieldView = ArrayUtil::First(Resources::FindObjectsOfTypeAll<InputFieldView*>(), [](InputFieldView* x) { 
            return to_utf8(csstrtostr(x->get_name())) == "GuestNameInputField"; });
        GameObject* gameObj = Object::Instantiate(originalfieldView->get_gameObject(), parent, false);
        gameObj->AddComponent<LayoutElement*>()->set_preferredHeight(8.0f);
        gameObj->GetComponent<RectTransform*>()->set_anchoredPosition(anchoredPosition);

        InputFieldView* fieldView = gameObj->GetComponent<InputFieldView*>();
        fieldView->useGlobalKeyboard = true;
        fieldView->keyboardPositionOffset = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
        GameObject* placeholder = fieldView->placeholderText;
        Object::Destroy(placeholder->GetComponent<LocalizedTextMeshProUGUI*>());
        placeholder->GetComponent<TextMeshProUGUI*>()->SetText(il2cpp_utils::createcsstr(settingsName));
        fieldView->SetText(il2cpp_utils::createcsstr(currentValue));
        fieldView->onValueChanged = InputFieldView::InputFieldChanged::New_ctor();
        if(onValueChange)
            fieldView->onValueChanged->AddListener(il2cpp_utils::MakeAction<UnityAction_1<InputFieldView*>>(il2cpp_functions::class_get_type(classof(UnityAction_1<InputFieldView*>*)), onValueChange, +[](UnityAction_1<Il2CppString*>* onValueChange, InputFieldView* fieldView) { onValueChange->Invoke(fieldView->get_text()); }));
        return fieldView;
    }

}
