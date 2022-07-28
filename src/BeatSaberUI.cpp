#include <fstream>
#include "BeatSaberUI.hpp"
#include "InternalBeatSaberUI.hpp"

#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Components/ScrollViewContent.hpp"
#include "CustomTypes/Components/MainThreadScheduler.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenManager.hpp"
#include "CustomTypes/Components/List/QuestUITableView.hpp"
#include "CustomTypes/Components/WeakPtrGO.hpp"
#include "CustomTypes/Components/ProgressBar/ProgressBar.hpp"

#include "GlobalNamespace/UIKeyboardManager.hpp"
#include "GlobalNamespace/BoolSettingsController.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "GlobalNamespace/ReleaseInfoViewController.hpp"
#include "GlobalNamespace/ColorPickerButtonController.hpp"
#include "GlobalNamespace/HSVPanelController.hpp"
#include "GlobalNamespace/HapticFeedbackController.hpp"
#include "GlobalNamespace/MenuShockwave.hpp"
#include "GlobalNamespace/LevelCollectionTableView.hpp"

#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/CanvasGroup.hpp"
#include "UnityEngine/AdditionalCanvasShaderChannels.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/UI/RectMask2D.hpp"
#include "UnityEngine/UI/ScrollRect.hpp"
#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "HMUI/Touchable.hpp"
#include "HMUI/HoverHintController.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableView_CellsGroup.hpp"
#include "HMUI/TableView_ScrollPositionType.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/TextPageScrollView.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "HMUI/InputFieldView_InputFieldChanged.hpp"
#include "HMUI/UIKeyboard.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"
#include "HMUI/EventSystemListener.hpp"
#include "HMUI/DropdownWithTableView.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"
#include "HMUI/TimeSlider.hpp"
#include "HMUI/ColorGradientSlider.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "HMUI/HoverTextSetter.hpp"

#include "VRUIControls/VRGraphicRaycaster.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"

#include "System/Convert.hpp"
#include "System/Action_2.hpp"
#include "System/Action.hpp"
#include "System/Collections/Generic/HashSet_1.hpp"

#include "Libraries/HM/HMLib/VR/HapticPresetSO.hpp"

#include "Zenject/DiContainer.hpp"

#include "customlogger.hpp"
#include "Sprites/carats.hpp"
#define DEFAULT_BUTTONTEMPLATE "PracticeButton"

#include "custom-types/shared/delegate.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace TMPro;
using namespace HMUI;
using namespace Polyglot;
using namespace VRUIControls;
using namespace Zenject;

#define MakeDelegate(DelegateType, varName) (custom_types::MakeDelegate<DelegateType>(classof(DelegateType), varName))

using HapticPresetSO = Libraries::HM::HMLib::VR::HapticPresetSO;

namespace QuestUI::BeatSaberUI {

    GameObject* beatSaberUIObject = nullptr;
    GameObject* dropdownListPrefab = nullptr;
    GameObject* modalPrefab = nullptr;
    
    void SetupPersistentObjects() {
        getLogger().info("SetupPersistentObjects");
        if(!beatSaberUIObject) {
            static ConstString name("BeatSaberUIObject");
            beatSaberUIObject = GameObject::New_ctor(name);
            GameObject::DontDestroyOnLoad(beatSaberUIObject);
            beatSaberUIObject->AddComponent<MainThreadScheduler*>();
        }
        if(!dropdownListPrefab) {
            GameObject* search = Resources::FindObjectsOfTypeAll<SimpleTextDropdown*>().First([](SimpleTextDropdown* x) { 
                    return x->get_transform()->get_parent()->get_name() == "NormalLevels"; 
                }
            )->get_transform()->get_parent()->get_gameObject();
            dropdownListPrefab = Object::Instantiate(search, beatSaberUIObject->get_transform(), false);
            static ConstString name("QuestUIDropdownListPrefab");
            dropdownListPrefab->set_name(name);
            dropdownListPrefab->SetActive(false);
        }
        if (!modalPrefab) {
            GameObject* search = Resources::FindObjectsOfTypeAll<ModalView*>().First([](ModalView* x) { 
                    return x->get_transform()->get_name() == "DropdownTableView";
                }
            )->get_gameObject();
            modalPrefab = Object::Instantiate(search, beatSaberUIObject->get_transform(), false);
            
            modalPrefab->GetComponent<ModalView*>()->presentPanelAnimations = search->GetComponent<ModalView*>()->presentPanelAnimations;
            modalPrefab->GetComponent<ModalView*>()->dismissPanelAnimation = search->GetComponent<ModalView*>()->dismissPanelAnimation;

            static ConstString name("QuestUIModalPrefab");
            modalPrefab->set_name(name);
            modalPrefab->SetActive(false);
        }
    }

    void AddModInfoText(ModSettingsInfos::ModSettingsInfo& info) {
        VerticalLayoutGroup* layout = CreateVerticalLayoutGroup(info.viewController->get_rectTransform());
        layout->get_rectTransform()->set_position(UnityEngine::Vector3(0.0f, -39.0f, -24.0f));
        layout->get_rectTransform()->set_eulerAngles(UnityEngine::Vector3(60.0f, 0.0f, 0.0f));
        GameObject* layoutGameObject = layout->get_gameObject();
        layoutGameObject->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
        static ConstString backgroundName("round-rect-panel");
        layoutGameObject->AddComponent<Backgroundable*>()->ApplyBackground(backgroundName);
        layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 4, 2, 2));
        TextMeshProUGUI* modInfoText = CreateText(layout->get_transform(), info.modInfo.id + " : v" + info.modInfo.version);
        modInfoText->set_alignment(TextAlignmentOptions::Center);
        modInfoText->set_fontSize(4.8f);
    }

    #define CacheNotFoundWarningLog(type) getLogger().warning("Can't find '" #type "'! (This shouldn't happen and can cause unexpected behaviour)")

    MainFlowCoordinator* mainFlowCoordinator = nullptr;
    MainFlowCoordinator* GetMainFlowCoordinator() {
        if(!mainFlowCoordinator)
            mainFlowCoordinator = Object::FindObjectOfType<MainFlowCoordinator*>();
        if(!mainFlowCoordinator)
            CacheNotFoundWarningLog(MainFlowCoordinator);
        return mainFlowCoordinator;
    }

    TMP_FontAsset* mainTextFont = nullptr;
    TMP_FontAsset* GetMainTextFont() {
        if(!mainTextFont)
            mainTextFont = Resources::FindObjectsOfTypeAll<TMP_FontAsset*>().FirstOrDefault([](TMP_FontAsset* x) { return x->get_name() == "Teko-Medium SDF"; });
        if(!mainTextFont)
            CacheNotFoundWarningLog(MainTextFont);
        return mainTextFont;
    }

    Material* mainUIFontMaterial = nullptr;
    Material* GetMainUIFontMaterial()
    {
        if(!mainUIFontMaterial)
            mainUIFontMaterial = Resources::FindObjectsOfTypeAll<Material*>().FirstOrDefault([](Material* x) { return x->get_name() == "Teko-Medium SDF Curved Softer"; });
        if(!mainUIFontMaterial)
            CacheNotFoundWarningLog(MainUIFontMaterial);
        return mainUIFontMaterial;
    }

    Sprite* editIcon = nullptr;
    Sprite* GetEditIcon() {
        if(!editIcon)
            editIcon = Resources::FindObjectsOfTypeAll<Image*>().First([](Image* x) { return x->get_sprite() && x->get_sprite()->get_name() == "EditIcon"; })->get_sprite();
        if(!editIcon)
            CacheNotFoundWarningLog(EditIcon);
        return editIcon;
    }

    PhysicsRaycasterWithCache* physicsRaycaster = nullptr;
    PhysicsRaycasterWithCache* GetPhysicsRaycasterWithCache()
    {
        if(!physicsRaycaster)
            physicsRaycaster = Resources::FindObjectsOfTypeAll<MainMenuViewController*>().First()->GetComponent<VRGraphicRaycaster*>()->physicsRaycaster;
        if(!physicsRaycaster)
            CacheNotFoundWarningLog(PhysicsRaycasterWithCache);
        return physicsRaycaster;
    }

    IVRPlatformHelper* platformHelper = nullptr;
    IVRPlatformHelper* GetIVRPlatformHelper()
    {
        if (!platformHelper)
            platformHelper = Resources::FindObjectsOfTypeAll<LevelCollectionTableView*>().First()->GetComponentInChildren<ScrollView*>()->platformHelper;
        if (!platformHelper)
            CacheNotFoundWarningLog(IVRPlatformHelper);
        return platformHelper;
    }

    DiContainer* diContainer = nullptr;
    DiContainer* GetDiContainer()
    {
        if(!diContainer)
            diContainer = Resources::FindObjectsOfTypeAll<TextSegmentedControl*>().FirstOrDefault([](TextSegmentedControl* x) { return x->get_transform()->get_parent()->get_name() == "PlayerStatisticsViewController" && x->container; })->container;
        if(!diContainer)
            CacheNotFoundWarningLog(DiContainer);
        return diContainer;
    }

    Material* mat_UINoGlow = nullptr;
    Material* NoGlowMaterial() {
        if (!mat_UINoGlow) {
            mat_UINoGlow = Resources::FindObjectsOfTypeAll<Material*>().First([](Material* x)
                { return x->get_name() == "UINoGlow"; });
        }

        return mat_UINoGlow;
    }

    HapticPresetSO* hapticFeedbackPresetSO = nullptr;
    HapticPresetSO* HapticPreset() {
        if (!hapticFeedbackPresetSO)
        {
            hapticFeedbackPresetSO = UnityEngine::ScriptableObject::CreateInstance<HapticPresetSO*>();
            hapticFeedbackPresetSO->duration = 0.01f;
            hapticFeedbackPresetSO->strength = 0.75f;
            hapticFeedbackPresetSO->frequency = 0.5f;
        }
        return hapticFeedbackPresetSO;
    }

    Signal* shockwaveSignal = nullptr;
    Signal* ShockwaveSignal() {
        if (!shockwaveSignal) {
            try
            {
                auto menuShockWave = Resources::FindObjectsOfTypeAll<GlobalNamespace::MenuShockwave*>().First();
                shockwaveSignal = menuShockWave->buttonClickEvents.Last();
                
            }
            catch(const std::exception& e)
            {
                getLogger().error("%s", e.what());
            }
        }

        return shockwaveSignal;
    }

    HapticFeedbackController* hapticController = nullptr;
    HapticFeedbackController* HapticController() {
        if (!hapticController) {
            hapticController = UnityEngine::Object::FindObjectOfType<GlobalNamespace::HapticFeedbackController*>();
        }
        return hapticController;
    }

    void ClearCache() {
        getLogger().info("Clearing Cache!");
        mainFlowCoordinator = nullptr;
        mainTextFont = nullptr;
        mainUIFontMaterial = nullptr;
        editIcon = nullptr;
        physicsRaycaster = nullptr;
        platformHelper = nullptr;
        diContainer = nullptr;

        mat_UINoGlow = nullptr;
        hapticFeedbackPresetSO = nullptr;
        shockwaveSignal = nullptr;
        hapticController = nullptr;
        QuestUI::WeakPtrHolder::goComponentMap.clear();
    }

    GameObject* CreateCanvas() {
        static ConstString name("QuestUICanvas");
        GameObject* go = GameObject::New_ctor(name);
        go->set_layer(5);
        Canvas* cv = go->AddComponent<Canvas*>();
        cv->set_additionalShaderChannels(AdditionalCanvasShaderChannels::TexCoord1 | AdditionalCanvasShaderChannels::TexCoord2);
        cv->set_sortingOrder(4);
        
        CanvasScaler* scaler = go->AddComponent<CanvasScaler*>();
        scaler->set_scaleFactor(1.0f);
        scaler->set_dynamicPixelsPerUnit(3.44f);
        scaler->set_referencePixelsPerUnit(10.0f);

        auto* physicsRaycaster = GetPhysicsRaycasterWithCache();
        if(physicsRaycaster)
            go->AddComponent<VRGraphicRaycaster*>()->physicsRaycaster = physicsRaycaster;

        RectTransform* rectTransform = go->GetComponent<RectTransform*>();
        float scale = 1.5f*0.02f; //Wrapper->ScreenSystem: 1.5 Wrapper->ScreenSystem->ScreenContainer: 0.02
        rectTransform->set_localScale(UnityEngine::Vector3(scale, scale, scale));
        return go;
    }

    static WeakPtrGO<Canvas> innerCanvasTemplate;

    static Canvas* getCanvasTemplate() {
        if (!innerCanvasTemplate)
            innerCanvasTemplate = Resources::FindObjectsOfTypeAll<Canvas *>().FirstOrDefault([](Canvas *x) {
                return x->get_name() == "DropdownTableView";
            });

        return innerCanvasTemplate.getInner();
    }

    ViewController* CreateViewController(System::Type* type) {
        static ConstString name("QuestUIViewController");
        GameObject* go = GameObject::New_ctor(name);

        Canvas* cv = go->AddComponent<Canvas*>();


        auto cvCopy = getCanvasTemplate();

        cv->set_additionalShaderChannels(cvCopy->get_additionalShaderChannels());
        cv->set_overrideSorting(cvCopy->get_overrideSorting());
        cv->set_pixelPerfect(cvCopy->get_pixelPerfect());
        cv->set_referencePixelsPerUnit(cvCopy->get_referencePixelsPerUnit());
        cv->set_renderMode(cvCopy->get_renderMode());
        cv->set_scaleFactor(cvCopy->get_scaleFactor());
        cv->set_sortingLayerID(cvCopy->get_sortingLayerID());
        cv->set_sortingOrder(cvCopy->get_sortingOrder());
        cv->set_worldCamera(cvCopy->get_worldCamera());

        auto* physicsRaycaster = GetPhysicsRaycasterWithCache();
        if(physicsRaycaster)
            go->AddComponent<VRGraphicRaycaster*>()->physicsRaycaster = physicsRaycaster;
        go->AddComponent<CanvasGroup*>();
        ViewController* vc = reinterpret_cast<ViewController*>(go->AddComponent(type));

        RectTransform* rectTransform = go->GetComponent<RectTransform*>();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        go->SetActive(false);
        return vc;
    }
    
    FlowCoordinator* CreateFlowCoordinator(System::Type* type) {
        static ConstString name("QuestUIFlowCoordinator");
        FlowCoordinator* flowCoordinator = reinterpret_cast<FlowCoordinator*>(GameObject::New_ctor(name)->AddComponent(type));
        flowCoordinator->baseInputModule = GetMainFlowCoordinator()->baseInputModule;
        return flowCoordinator;
    }

    TextMeshProUGUI* CreateText(Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, true, anchoredPosition);
    }

    TextMeshProUGUI* CreateText(Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        return CreateText(parent, text, true, anchoredPosition, sizeDelta);
    }

    TextMeshProUGUI* CreateText(Transform* parent, StringW text, bool italic) {
        return CreateText(parent, text, italic, UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, italic, anchoredPosition, UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static ConstString name("QuestUIText");
        GameObject* gameObj = GameObject::New_ctor(name);
        gameObj->SetActive(false);

        CurvedTextMeshPro* textMesh = gameObj->AddComponent<CurvedTextMeshPro*>();
        RectTransform* rectTransform = textMesh->get_rectTransform();
        rectTransform->SetParent(parent, false);
        textMesh->set_font(GetMainTextFont());
        textMesh->set_fontSharedMaterial(GetMainUIFontMaterial());
        if (italic) textMesh->set_fontStyle(TMPro::FontStyles::Italic);
        textMesh->set_text(text);
        textMesh->set_fontSize(4.0f);
        textMesh->set_color(UnityEngine::Color::get_white());
        textMesh->set_richText(true);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchoredPosition(anchoredPosition);
        rectTransform->set_sizeDelta(sizeDelta);
        
        gameObj->AddComponent<LayoutElement*>();

        gameObj->SetActive(true);
        return textMesh;
    }

    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick)
    {
        return CreateClickableText(parent, text, true, anchoredPosition, onClick);
    }

    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick)
    {
        return CreateClickableText(parent, text, true, anchoredPosition, sizeDelta, onClick);
    }

    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, bool italic, std::function<void()> onClick)
    {
        return CreateClickableText(parent, text, italic, UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(60.0f, 10.0f), onClick);
    }

    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick)
    {
        return CreateClickableText(parent, text, italic, anchoredPosition, UnityEngine::Vector2(60.0f, 10.0f), onClick);
    }

    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick)
    {
        static ConstString name("QuestUIText");
        GameObject* gameObj = GameObject::New_ctor(name);
        gameObj->SetActive(false);

        ClickableText* textMesh = gameObj->AddComponent<ClickableText*>();
        RectTransform* rectTransform = textMesh->get_rectTransform();
        rectTransform->SetParent(parent, false);
        textMesh->set_font(GetMainTextFont());
        textMesh->set_fontSharedMaterial(GetMainUIFontMaterial());
        if (italic) text = StringW(u"<i>" + text + u"</i>");
        textMesh->set_text(text);
        textMesh->set_fontSize(4.0f);
        textMesh->set_color(UnityEngine::Color::get_white());
        textMesh->set_richText(true);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchoredPosition(anchoredPosition);
        rectTransform->set_sizeDelta(sizeDelta);
        
        gameObj->AddComponent<LayoutElement*>();

        if (onClick)
            textMesh->get_onPointerClickEvent() += [onClick](auto _){ onClick(); };

        try
        {
            auto menuShockWave = Resources::FindObjectsOfTypeAll<GlobalNamespace::MenuShockwave*>().First();
            auto buttonClickedSignal = menuShockWave->buttonClickEvents.Last();
            textMesh->buttonClickedSignal = buttonClickedSignal;
        }
        catch(const std::exception& e)
        {
            getLogger().error("%s", e.what());
        }

        textMesh->hapticFeedbackController = HapticController();
        textMesh->hapticFeedbackPresetSO = HapticPreset();

        gameObj->SetActive(true);
        return textMesh;
    }

    void SetButtonText(Button* button, StringW text) {
        LocalizedTextMeshProUGUI* localizer = button->GetComponentInChildren<LocalizedTextMeshProUGUI*>();
        if (localizer)
            Object::Destroy(localizer);
        TextMeshProUGUI* textMesh = button->GetComponentInChildren<TextMeshProUGUI*>();
        if(textMesh)
            textMesh->set_text(text);
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
        auto array = button->GetComponentsInChildren<Image*>();
        if(array.Length() > 1)
            array.First([](Image* x) { return x->get_name() == "Icon";})->set_sprite(icon);
    }

    void SetButtonBackground(Button* button, Sprite* background) {
        auto array = button->GetComponentsInChildren<Image*>();
        if(array.Length() > 0)
            array[0]->set_sprite(background);
    }

    void SetButtonSprites(UnityEngine::UI::Button* button, UnityEngine::Sprite* inactive, UnityEngine::Sprite* active) {
        // make sure the textures are set to clamp
        inactive->get_texture()->set_wrapMode(TextureWrapMode::Clamp);
        active->get_texture()->set_wrapMode(TextureWrapMode::Clamp);

        ButtonSpriteSwap* spriteSwap = button->GetComponent<ButtonSpriteSwap*>();

        // setting the sprites
        spriteSwap->highlightStateSprite = active;
        spriteSwap->pressedStateSprite = active;

        spriteSwap->disabledStateSprite = inactive;
        spriteSwap->normalStateSprite = inactive;
    }

    Button* CreateUIButton(Transform* parent, StringW buttonText, std::string_view buttonTemplate, std::function<void()> onClick) {
        static std::unordered_map<std::string, WeakPtrGO<Button>> buttonCopyMap;
        auto& buttonCopy = buttonCopyMap[std::string(buttonTemplate)];
        if (!buttonCopy) {
            buttonCopy = Resources::FindObjectsOfTypeAll<Button*>().LastOrDefault([&buttonTemplate](auto* x) { return x->get_name() == buttonTemplate; });
        }

        Button* button = Object::Instantiate(buttonCopy.getInner(), parent, false);
        button->set_onClick(Button::ButtonClickedEvent::New_ctor());
        static ConstString name("QuestUIButton");
        button->set_name(name);
        if(onClick)
            button->get_onClick()->AddListener(MakeDelegate(UnityAction*, onClick));

        LocalizedTextMeshProUGUI* localizer = button->GetComponentInChildren<LocalizedTextMeshProUGUI*>();
        if (localizer != nullptr)
            GameObject::Destroy(localizer);
        ExternalComponents* externalComponents = button->get_gameObject()->AddComponent<ExternalComponents*>();

        TextMeshProUGUI* textMesh = button->GetComponentInChildren<TextMeshProUGUI*>();
        if (textMesh)
        {
            textMesh->set_richText(true);
            textMesh->set_alignment(TextAlignmentOptions::Center);
            externalComponents->Add(textMesh);
        }
        RectTransform* rectTransform = (RectTransform*)button->get_transform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_pivot(UnityEngine::Vector2(0.5f, 0.5f));

        textMesh->set_text(buttonText);

        HorizontalLayoutGroup* horiztonalLayoutGroup = button->GetComponentInChildren<HorizontalLayoutGroup*>();
        if (horiztonalLayoutGroup != nullptr)
            externalComponents->Add(horiztonalLayoutGroup);
        
        // if the original button was for some reason not interactable, now it will be
        button->set_interactable(true);
        button->get_gameObject()->SetActive(true);
        return button;
    }

    Button* CreateUIButton(Transform* parent, StringW buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick) {
        Button* button = CreateUIButton(parent, buttonText, buttonTemplate, onClick);
        button->GetComponent<RectTransform*>()->set_anchoredPosition(anchoredPosition);
        return button;
    }

    Button* CreateUIButton(Transform* parent, StringW buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick) {
        Button* button = CreateUIButton(parent, buttonText, buttonTemplate, anchoredPosition, onClick);
        button->GetComponent<RectTransform*>()->set_sizeDelta(sizeDelta);
        LayoutElement* layoutElement = button->GetComponent<LayoutElement*>();
        if(!layoutElement)
            layoutElement = button->get_gameObject()->AddComponent<LayoutElement*>();
        layoutElement->set_minWidth(sizeDelta.x);
        layoutElement->set_minHeight(sizeDelta.y);
        layoutElement->set_preferredWidth(sizeDelta.x);
        layoutElement->set_preferredHeight(sizeDelta.y);
        layoutElement->set_flexibleWidth(sizeDelta.x);
        layoutElement->set_flexibleHeight(sizeDelta.y);
        return button;
    }

    Button* CreateUIButton(Transform* parent, StringW buttonText, std::function<void()> onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, onClick);
    }

    Button* CreateUIButton(Transform* parent, StringW buttonText, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, anchoredPosition, onClick);
    }

    Button* CreateUIButton(Transform* parent, StringW buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, anchoredPosition, sizeDelta, onClick);
    }


    Toggle * CreateModifierButton(UnityEngine::Transform *parent, StringW buttonText, bool currentValue, UnityEngine::Sprite *iconSprite, std::function<void(bool)> const &onClick, UnityEngine::Vector2 anchoredPosition) {
        static WeakPtrGO <GameplayModifierToggle> toggleTemplate;
        if (!toggleTemplate)
            toggleTemplate = Resources::FindObjectsOfTypeAll<GameplayModifierToggle *>().First( [](GameplayModifierToggle * x) { return x->get_name() == "InstaFail"; });

        GameplayModifierToggle *baseModifier = Object::Instantiate((GameplayModifierToggle *) toggleTemplate, parent, false);
        static ConstString baseModifierName("BSMLModifier");
        baseModifier->set_name(baseModifierName);

        GameObject *gameObject = baseModifier->get_gameObject();
        gameObject->SetActive(false);

        Object::Destroy(baseModifier);
        Object::Destroy(gameObject->GetComponent<HMUI::HoverTextSetter *>());

        Object::Destroy(gameObject->get_transform()->Find("Multiplier")->get_gameObject());


        GameObject *nameText = gameObject->get_transform()->Find("Name")->get_gameObject();
        TMPro::TextMeshProUGUI *text = nameText->GetComponent<TMPro::TextMeshProUGUI *>();
        text->set_text(buttonText);

        auto *localizer = gameObject->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI *>();
        if (localizer != nullptr)
            GameObject::Destroy(localizer);

        Image *image = gameObject->get_transform()->Find("Icon")->GetComponent<Image *>();

        auto externalComponents = gameObject->AddComponent<ExternalComponents *>();
        externalComponents->Add(text);

        if (iconSprite) {
            image->set_sprite(iconSprite);
            externalComponents->Add(image);
        } else {
            Object::Destroy(image);
        }

        auto toggle = gameObject->GetComponent<Toggle *>();
        toggle->onValueChanged = Toggle::ToggleEvent::New_ctor();
        toggle->set_interactable(true);
        toggle->set_isOn(currentValue);
        if (onClick)
            toggle->onValueChanged->AddListener(MakeDelegate(UnityAction_1<bool>*, onClick));

        auto *rectTransform = gameObject->GetComponent<RectTransform *>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        gameObject->SetActive(true);

        return toggle;
    }

    QuestUI::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick)
    {
        static ConstString name("QuestUIClickableImage");
        auto go = GameObject::New_ctor(name);

        auto image = go->AddComponent<QuestUI::ClickableImage*>();
        
        image->set_material(NoGlowMaterial());

        go->get_transform()->SetParent(parent, false);

        auto rectTransform = image->get_rectTransform();
        rectTransform->set_sizeDelta(sizeDelta);
        rectTransform->set_anchoredPosition(anchoredPosition);
        image->set_sprite(sprite);

        go->AddComponent<LayoutElement*>();

        if (onClick)
            image->get_onPointerClickEvent() += [onClick](auto _){ onClick(); };
        
        image->buttonClickedSignal = ShockwaveSignal();
        image->hapticFeedbackController = HapticController();
        image->hapticFeedbackPresetSO = HapticPreset();

        return image;
    }

    QuestUI::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, std::function<void()> onClick)
    {
        return CreateClickableImage(parent, sprite, {0, 0}, {0, 0}, onClick);
    }


    ImageView* CreateImage(Transform* parent, Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static ConstString name("QuestUIImage");
        GameObject* gameObj = GameObject::New_ctor(name);
        ImageView* image = gameObj->AddComponent<ImageView*>();
        image->set_material(NoGlowMaterial());
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


    Sprite* FileToSprite(std::string_view filePath)
    {
        std::ifstream instream(filePath, std::ios::in | std::ios::binary | std::ios::ate);
        unsigned long size = instream.tellg();
        instream.seekg(0, instream.beg);

        //std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
        //Array<uint8_t>* bytes = il2cpp_utils::vectorToArray(data);
        
        // this way we skip the copy step from vector to array, and just read directly into the array, making it quicker
        Array<uint8_t>* bytes = Array<uint8_t>::NewLength(size);
        instream.read(reinterpret_cast<char*>(bytes->values), size);
        
        return ArrayToSprite(bytes);
    }

    Sprite* VectorToSprite(std::vector<uint8_t> bytes) {
        return ArrayToSprite(il2cpp_utils::vectorToArray(bytes));
    }

    Sprite* ArrayToSprite(ArrayW<uint8_t> bytes)
    {
        Texture2D* texture = Texture2D::New_ctor(0, 0, TextureFormat::RGBA32, false, false);
        if (ImageConversion::LoadImage(texture, bytes, false)) {
            texture->set_wrapMode(TextureWrapMode::Clamp);
            return Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), UnityEngine::Vector2(0.5f,0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, UnityEngine::Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        }
        return nullptr;
    }

    GridLayoutGroup* CreateGridLayoutGroup(Transform* parent) {
        static ConstString name("QuestUIGridLayoutGroup");
        GameObject* gameObject = GameObject::New_ctor(name);
        GridLayoutGroup* layout = gameObject->AddComponent<GridLayoutGroup*>();
        gameObject->AddComponent<ContentSizeFitter*>();
        gameObject->AddComponent<Backgroundable*>();

        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

        gameObject->AddComponent<LayoutElement*>();
        return layout;
    }
    
    HorizontalLayoutGroup* CreateHorizontalLayoutGroup(Transform* parent) {
        static ConstString name("QuestUIHorizontalLayoutGroup");
        GameObject* gameObject = GameObject::New_ctor(name);
        HorizontalLayoutGroup* layout = gameObject->AddComponent<HorizontalLayoutGroup*>();
        gameObject->AddComponent<Backgroundable*>();

        ContentSizeFitter* contentSizeFitter = gameObject->AddComponent<ContentSizeFitter*>();
        contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        
        gameObject->AddComponent<LayoutElement*>();
        return layout;
    }
    
    VerticalLayoutGroup* CreateVerticalLayoutGroup(Transform* parent) {
        static ConstString name("QuestUIVerticalLayoutGroup");
        GameObject* gameObject = GameObject::New_ctor(name);
        VerticalLayoutGroup* layout = gameObject->AddComponent<VerticalLayoutGroup*>();
        gameObject->AddComponent<Backgroundable*>();

        ContentSizeFitter* contentSizeFitter = gameObject->AddComponent<ContentSizeFitter*>();
        contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);

        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        
        gameObject->AddComponent<LayoutElement*>();
        return layout;
    }

    //https://github.com/monkeymanboy/BeatSaberMarkupLanguage/blob/03fc83884a829deac26e4f0075470d9b4675897b/BeatSaberMarkupLanguage/Tags/ModifierContainerTag.cs#L17-L28
    UnityEngine::UI::VerticalLayoutGroup *CreateModifierContainer(UnityEngine::Transform *parent) {
        UnityEngine::UI::VerticalLayoutGroup* layout = CreateVerticalLayoutGroup(parent);

        layout->set_padding(RectOffset::New_ctor(3, 3, 2, 2));
        layout->set_childControlHeight(false);
        layout->set_childForceExpandHeight(false);

        layout->get_gameObject()->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

        RectTransform* rectTransform = layout->get_gameObject()->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition({0, 3});
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(54.0f, 0.0f));

        return layout;
    }

    Toggle* CreateToggle(Transform* parent, StringW text, std::function<void(bool)> onValueChange)
    {
        return CreateToggle(parent, text, false, onValueChange);
    }

    Toggle* CreateToggle(Transform* parent, StringW text, bool currentValue, std::function<void(bool)> onValueChange)
    {
        return CreateToggle(parent, text, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    Toggle* CreateToggle(Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onValueChange)
    {
        return CreateToggle(parent, text, false, anchoredPosition, onValueChange);
    }
    
    Toggle* CreateToggle(Transform* parent, StringW text, bool currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onValueChange)
    {
        static WeakPtrGO<GameObject> toggleCopy;
        if (!toggleCopy) {
            auto foundToggle = Resources::FindObjectsOfTypeAll<Toggle*>().FirstOrDefault([](auto x) { return x->get_transform()->get_parent()->get_gameObject()->get_name() == "Fullscreen"; });
            toggleCopy = foundToggle ? foundToggle->get_transform()->get_parent()->get_gameObject() : nullptr;
        }


        GameObject* gameObject = Object::Instantiate(toggleCopy.getInner(), parent, false);
        static ConstString nameTextName("NameText");
        GameObject* nameText = gameObject->get_transform()->Find(nameTextName)->get_gameObject();
        Object::Destroy(gameObject->GetComponent<BoolSettingsController*>());

        static ConstString name("QuestUICheckboxSetting");
        gameObject->set_name(name);

        gameObject->SetActive(false);

        Object::Destroy(nameText->GetComponent<LocalizedTextMeshProUGUI*>());
        
        Toggle* toggle = gameObject->GetComponentInChildren<Toggle*>();
        toggle->set_interactable(true);
        toggle->set_isOn(currentValue);
        toggle->onValueChanged = Toggle::ToggleEvent::New_ctor();
        if(onValueChange)
            toggle->onValueChanged->AddListener(MakeDelegate(UnityAction_1<bool>*, onValueChange));
        TextMeshProUGUI* textMesh = nameText->GetComponent<TextMeshProUGUI*>();
        textMesh->SetText(text);
        textMesh->set_richText(true);
        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        LayoutElement* layout = gameObject->GetComponent<LayoutElement*>();
        layout->set_preferredWidth(90.0f);
        gameObject->SetActive(true);
        return toggle;
    }

    /*GameObject* CreateLoadingIndicator(Transform* parent, UnityEngine::Vector2 anchoredPosition)
    {
        GameObject* loadingIndicator = GameObject::Instantiate(Resources::FindObjectsOfTypeAll<GameObject*>().First([](GameObject* x){ return x->get_name() == "LoadingIndicator"; }), parent, false);
        static ConstString name("QuestUILoadingIndicator");
        loadingIndicator->set_name(name);

        loadingIndicator->AddComponent<LayoutElement*>();

        RectTransform* rectTransform = loadingIndicator->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        return loadingIndicator;
    }*/

    HoverHint* AddHoverHint(GameObject* gameObject, StringW text){
        HoverHint* hoverHint = gameObject->AddComponent<HoverHint*>();
        hoverHint->set_text(text);

        static WeakPtrGO<HoverHintController> hoverHintController;
        if (!hoverHintController) {
            hoverHintController = Resources::FindObjectsOfTypeAll<HoverHintController*>().First();
        }

        hoverHint->hoverHintController = hoverHintController;
        return hoverHint;
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, StringW text, int decimals, float increment, float currentValue, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, StringW text, int decimals, float increment, float currentValue, float minValue, float maxValue, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, minValue, maxValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, StringW text, int decimals, float increment, float currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, false, false, 0.0f, 0.0f, anchoredPosition, onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, StringW text, int decimals, float increment, float currentValue, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, true, true, minValue, maxValue, anchoredPosition, onValueChange);
    }

    static WeakPtrGO<FormattedFloatListSettingsValueController> innerFormattedListSettingsValueController;

    FormattedFloatListSettingsValueController* getValueTemplate() {
        if (!innerFormattedListSettingsValueController) {
            innerFormattedListSettingsValueController = Resources::FindObjectsOfTypeAll<FormattedFloatListSettingsValueController*>().First([](FormattedFloatListSettingsValueController* x){ return x->get_name() == "VRRenderingScale"; });
        }

        return innerFormattedListSettingsValueController.getInner();
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, StringW text, int decimals, float increment, float currentValue, bool hasMin, bool hasMax, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange) {
        FormattedFloatListSettingsValueController* baseSetting = Object::Instantiate(getValueTemplate(), parent, false);
        static ConstString name("QuestUIIncDecSetting");
        baseSetting->set_name(name);
        
        GameObject* gameObject = baseSetting->get_gameObject();
        Object::Destroy(baseSetting);
        gameObject->SetActive(false);

        IncrementSetting* setting = gameObject->AddComponent<IncrementSetting*>();
        setting->Decimals = decimals;
        setting->Increment = increment;
        setting->HasMin = hasMin;
        setting->HasMax = hasMax;
        setting->MinValue = minValue;
        setting->MaxValue = maxValue;
        setting->CurrentValue = currentValue;
        setting->OnValueChange = std::move(onValueChange);
        Transform* child = gameObject->get_transform()->GetChild(1);
        setting->Text = child->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
        setting->Text->SetText(setting->GetRoundedString());
        setting->Text->set_richText(true);
        Button* decButton = child->GetComponentsInChildren<Button*>().First();
        Button* incButton = child->GetComponentsInChildren<Button*>().Last();
        decButton->set_interactable(true);
        incButton->set_interactable(true);
        decButton->get_onClick()->AddListener(MakeDelegate(UnityAction*, (std::function<void()>)[setting](){ setting->DecButtonPressed(); }));
        incButton->get_onClick()->AddListener(MakeDelegate(UnityAction*, (std::function<void()>)[setting](){ setting->IncButtonPressed(); }));
        
        child->GetComponent<RectTransform*>()->set_sizeDelta(UnityEngine::Vector2(40, 0));
        TextMeshProUGUI* textMesh = gameObject->GetComponentInChildren<TextMeshProUGUI*>();
        textMesh->SetText(text);
        textMesh->set_richText(true);
        gameObject->AddComponent<ExternalComponents*>()->Add(textMesh);

        Object::Destroy(textMesh->GetComponent<LocalizedTextMeshProUGUI*>());

        LayoutElement* layoutElement = gameObject->AddComponent<LayoutElement*>();
        layoutElement->set_preferredWidth(90.0f);

        RectTransform* rectTransform = gameObject->GetComponent<RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);

        gameObject->SetActive(true);    

        return setting;
    }

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW name, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange)
    {
        return CreateSliderSetting(parent, name, increment, value, minValue, maxValue, 1.0f, {0.0f, 0.0f}, onValueChange);
    }

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW name, float increment, float value, float minValue, float maxValue, float applyValueTime, std::function<void(float)> onValueChange)
    {
        return CreateSliderSetting(parent, name, increment, value, minValue, maxValue, applyValueTime, {0.0f, 0.0f}, onValueChange);
    }

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW name, float increment, float value, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange)
    {
        return CreateSliderSetting(parent, name, increment, value, minValue, maxValue, 1.0f, anchoredPosition, onValueChange);
    }

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW name, float increment, float value, float minValue, float maxValue, float applyValueTime, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange)
    {
        FormattedFloatListSettingsValueController* baseSetting = Object::Instantiate(getValueTemplate(), parent, false);
        static ConstString QuestUISliderSetting_cs("QuestUISliderSetting");
        baseSetting->set_name(QuestUISliderSetting_cs);

        auto gameObject = baseSetting->get_gameObject();

        auto sliderSetting = gameObject->AddComponent<QuestUI::SliderSetting*>();
        static ConstString ValuePicker_cs("ValuePicker");
        Object::Destroy(gameObject->get_transform()->Find(ValuePicker_cs)->get_gameObject());

        static WeakPtrGO<TimeSlider> timeSliderTemplate;

        if (!timeSliderTemplate) {
            timeSliderTemplate = Resources::FindObjectsOfTypeAll<TimeSlider *>().First([](auto s) {
                if (s->get_name() != "RangeValuesTextSlider") return false;
                auto parent = s->get_transform()->get_parent();
                if (!parent) return false;
                return parent->get_name() == "SongStart";
            });
        }

        sliderSetting->slider = Object::Instantiate(timeSliderTemplate.getInner(), gameObject->get_transform(), false);
        sliderSetting->Setup(minValue, maxValue, value, increment, applyValueTime, std::move(onValueChange));
        static ConstString QuestUISlider_cs("QuestUISlider");
        sliderSetting->slider->set_name(QuestUISlider_cs);
        sliderSetting->slider->GetComponentInChildren<TextMeshProUGUI*>()->set_enableWordWrapping(false);
        
        auto rectTransform = reinterpret_cast<RectTransform*>(sliderSetting->slider->get_transform());
        rectTransform->set_anchorMin(Vector2(1, 0));
        rectTransform->set_anchorMax(Vector2(1, 1));
        rectTransform->set_sizeDelta(Vector2(40, 0));
        rectTransform->set_pivot(Vector2(1, 0.5f));
        rectTransform->set_anchoredPosition(Vector2(0, 0));

        Object::Destroy(baseSetting);

        static ConstString NameText_cs("NameText"); 
        auto nameText = gameObject->get_transform()->Find(NameText_cs)->get_gameObject();
        Polyglot::LocalizedTextMeshProUGUI* localizedText = nameText->GetComponent<Polyglot::LocalizedTextMeshProUGUI*>();
        localizedText->set_enabled(false);
        localizedText->set_Key(System::String::_get_Empty());

        auto text = nameText->GetComponent<TextMeshProUGUI*>();
        text->set_text(name);

        auto externalComponents = gameObject->AddComponent<ExternalComponents*>();
        externalComponents->Add(text);
        externalComponents->Add(localizedText);

        gameObject->GetComponent<LayoutElement*>()->set_preferredWidth(90);

        gameObject->SetActive(true);
        gameObject->GetComponent<RectTransform*>()->set_anchoredPosition(anchoredPosition);
        return sliderSetting;
    }

    GameObject* CreateScrollView(Transform* parent) {
        static WeakPtrGO<TextPageScrollView> textScrollViewClone;
        if (!textScrollViewClone) {
            textScrollViewClone = Resources::FindObjectsOfTypeAll<ReleaseInfoViewController *>().First()->textPageScrollView;
        }
        auto textScrollView = Object::Instantiate(textScrollViewClone.getInner(), parent);
        static ConstString textScrollViewName("QuestUIScrollView");
        textScrollView->set_name(textScrollViewName);
        Button* pageUpButton = textScrollView->pageUpButton;
        Button* pageDownButton = textScrollView->pageDownButton;
        VerticalScrollIndicator* verticalScrollIndicator = textScrollView->verticalScrollIndicator; 
        RectTransform* viewport = textScrollView->viewport;

        auto* physicsRaycaster = GetPhysicsRaycasterWithCache();
        if(physicsRaycaster)
            viewport->get_gameObject()->AddComponent<VRGraphicRaycaster*>()->physicsRaycaster = physicsRaycaster;
        
        GameObject::Destroy(textScrollView->text->get_gameObject());
        GameObject* gameObject = textScrollView->get_gameObject();
        GameObject::Destroy(textScrollView);
        gameObject->SetActive(false);

        ScrollView* scrollView = gameObject->AddComponent<ScrollView*>();
        scrollView->pageUpButton = pageUpButton;
        scrollView->pageDownButton = pageDownButton;
        scrollView->verticalScrollIndicator = verticalScrollIndicator;
        scrollView->viewport = viewport;
        scrollView->platformHelper = GetIVRPlatformHelper();

        viewport->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        viewport->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));

        static ConstString parentObjName("QuestUIScrollViewContent");
        GameObject* parentObj = GameObject::New_ctor(parentObjName);
        parentObj->get_transform()->SetParent(viewport, false);

        ContentSizeFitter* contentSizeFitter = parentObj->AddComponent<ContentSizeFitter*>();
        contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

        VerticalLayoutGroup* verticalLayout = parentObj->AddComponent<VerticalLayoutGroup*>();
        verticalLayout->set_childForceExpandHeight(false);
        verticalLayout->set_childForceExpandWidth(false);
        verticalLayout->set_childControlHeight(true);
        verticalLayout->set_childControlWidth(true);
        verticalLayout->set_childAlignment(TextAnchor::UpperCenter);

        RectTransform* rectTransform = parentObj->GetComponent<RectTransform*>();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 1.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_pivot(UnityEngine::Vector2(0.5f, 1.0f));
        
        parentObj->AddComponent<ScrollViewContent*>()->scrollView = scrollView;

        static ConstString childName("QuestUIScrollViewContentContainer");
        GameObject* child = GameObject::New_ctor(childName);
        child->get_transform()->SetParent(rectTransform, false);

        VerticalLayoutGroup* layoutGroup = child->AddComponent<VerticalLayoutGroup*>();
        layoutGroup->set_childControlHeight(false);
        layoutGroup->set_childForceExpandHeight(false);
        layoutGroup->set_childAlignment(TextAnchor::LowerCenter);
        layoutGroup->set_spacing(0.5f);

        ExternalComponents* externalComponents = child->AddComponent<ExternalComponents*>();
        externalComponents->Add(scrollView);
        externalComponents->Add(scrollView->get_transform());
        externalComponents->Add(gameObject->AddComponent<LayoutElement*>());

        child->GetComponent<RectTransform*>()->set_sizeDelta(UnityEngine::Vector2(0.0f, -1.0f));

        scrollView->contentRectTransform = rectTransform;

        gameObject->SetActive(true);
        return child;
    }

    GameObject* CreateScrollableSettingsContainer(Transform* parent) {
        GameObject* content = CreateScrollView(parent);
        ExternalComponents* externalComponents = content->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, 0.0f));
        static ConstString name("QuestUIScrollableSettingsContainer");
        scrollTransform->get_gameObject()->set_name(name);
        return content;
    }

    InputFieldView* CreateStringSetting(Transform* parent, StringW settingsName, StringW currentValue, std::function<void(StringW)> onValueChange) {
        return CreateStringSetting(parent, settingsName, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    InputFieldView* CreateStringSetting(Transform* parent, StringW settingsName, StringW currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(StringW)> onValueChange) {
        return CreateStringSetting(parent, settingsName, currentValue, anchoredPosition, UnityEngine::Vector3(1337.0f, 1337.0f, 1337.0f), onValueChange);
    }
    
    InputFieldView* CreateStringSetting(Transform* parent, StringW settingsName, StringW currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector3 keyboardPositionOffset, std::function<void(StringW)> onValueChange) {
        static WeakPtrGO<InputFieldView> originalFieldView;
        if (!originalFieldView) {
            originalFieldView = Resources::FindObjectsOfTypeAll<InputFieldView *>().First(
            [](InputFieldView *x) {
                return x->get_name() == "GuestNameInputField";
            });
        }
        GameObject* gameObj = Object::Instantiate(originalFieldView->get_gameObject(), parent, false);
        static ConstString name("QuestUIStringSetting");
        gameObj->set_name(name);
        LayoutElement* layoutElement = gameObj->AddComponent<LayoutElement*>();
        layoutElement->set_preferredWidth(90.0f);
        layoutElement->set_preferredHeight(8.0f);
        gameObj->GetComponent<RectTransform*>()->set_anchoredPosition(anchoredPosition);

        InputFieldView* fieldView = gameObj->GetComponent<InputFieldView*>();
        fieldView->useGlobalKeyboard = true;
        fieldView->textLengthLimit = 128;
        fieldView->keyboardPositionOffset = keyboardPositionOffset;

        fieldView->Awake();

        Object::Destroy(fieldView->placeholderText->GetComponent<LocalizedTextMeshProUGUI*>());
        fieldView->placeholderText->GetComponent<TextMeshProUGUI*>()->SetText(settingsName);
        fieldView->SetText(currentValue);
        fieldView->onValueChanged = InputFieldView::InputFieldChanged::New_ctor();
        if(onValueChange) {
            fieldView->onValueChanged->AddListener(MakeDelegate(UnityAction_1<InputFieldView*>*,
                (std::function<void(InputFieldView*)>)[onValueChange](InputFieldView* fieldView){
                    onValueChange(fieldView->get_text());
                }));
        }
        return fieldView;
    }

    SimpleTextDropdown* CreateDropdownInternal(Transform* parent, StringW dropdownName, int selectedIndex, List<StringW>* values, std::function<void(SimpleTextDropdown*, int)> onValueChange)
    {
        GameObject* gameObj = Object::Instantiate(dropdownListPrefab, parent, false);
        static ConstString name("QuestUIDropdownList");
        gameObj->set_name(name);
        SimpleTextDropdown* dropdown = gameObj->GetComponentInChildren<SimpleTextDropdown*>();
        dropdown->get_gameObject()->SetActive(false);
        auto* physicsRaycaster = GetPhysicsRaycasterWithCache();
        if(physicsRaycaster)
            reinterpret_cast<VRGraphicRaycaster*>(dropdown->GetComponentInChildren<VRGraphicRaycaster*>(true))->physicsRaycaster = physicsRaycaster;
        
        reinterpret_cast<ModalView*>(dropdown->GetComponentInChildren<ModalView*>(true))->container = GetDiContainer();

        static ConstString labelName("Label");
        GameObject* labelObject = gameObj->get_transform()->Find(labelName)->get_gameObject();
        GameObject::Destroy(labelObject->GetComponent<LocalizedTextMeshProUGUI*>());
        labelObject->GetComponent<TextMeshProUGUI*>()->SetText(dropdownName);

        LayoutElement* layoutElement = gameObj->AddComponent<LayoutElement*>();
        layoutElement->set_preferredWidth(90.0f);
        layoutElement->set_preferredHeight(8.0f);
        dropdown->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<StringW>*>(values));
        dropdown->SelectCellWithIdx(selectedIndex);

        if(onValueChange) {
            using DelegateType = System::Action_2<DropdownWithTableView*, int>*;
            dropdown->add_didSelectCellWithIdxEvent(MakeDelegate(DelegateType, onValueChange));
        }

        dropdown->get_gameObject()->SetActive(true);
        gameObj->SetActive(true);
        return dropdown;
    }

    SimpleTextDropdown* CreateDropdown(Transform* parent, StringW dropdownName, StringW currentValue, std::vector<StringW> values, std::function<void(StringW)> onValueChange)
    {
        List<StringW>* list = List<StringW>::New_ctor();
        int selectedIndex = 0;
        for (int i = 0; i < values.size(); i++){
            auto value = values[i];
            if (currentValue == value)
                selectedIndex = i;
            list->Add(value);
        }
        std::function<void(SimpleTextDropdown*, int)> func = [onValueChange] (SimpleTextDropdown* dropdownWithTableView, int index) {
                onValueChange(reinterpret_cast<List<StringW>*>(dropdownWithTableView->texts)->get_Item(index));
            };
        return CreateDropdownInternal(parent, dropdownName, selectedIndex, list, func);
    }
    
    GameObject* CreateFloatingScreen(UnityEngine::Vector2 screenSize, UnityEngine::Vector3 position, UnityEngine::Vector3 rotation, float curvatureRadius, bool hasBackground, bool createHandle, int handleSide) {
        //Set up canvas components
        auto gameObject = CreateCanvas();
        auto manager = gameObject->AddComponent<FloatingScreenManager*>();
        auto screen = gameObject->AddComponent<FloatingScreen*>();
        if(createHandle) {
            screen->set_showHandle(true);
            screen->set_side(handleSide);
        }
        auto curvedCanvasSettings = gameObject->AddComponent<CurvedCanvasSettings*>();
        curvedCanvasSettings->SetRadius(curvatureRadius);

        //background
        if(hasBackground) {
            static ConstString backgroundGoName("bg");
            auto backgroundGo = UnityEngine::GameObject::New_ctor(backgroundGoName);
            backgroundGo->get_transform()->SetParent(gameObject->get_transform(), false);
            auto background = backgroundGo->AddComponent<Backgroundable*>();
            static ConstString backgroundName("round-rect-panel");
            background->ApplyBackgroundWithAlpha(backgroundName, 0.5f);
            screen->set_bgGo(backgroundGo);
        }

        //Transform
        auto transform = gameObject->get_transform();
        transform->set_position(position);
        transform->set_eulerAngles(rotation);
        screen->set_screenSize(screenSize);

        return gameObject;
    }

    ColorSetting* CreateColorPicker(Transform* parent, StringW text, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color)> onDone, std::function<void()> onCancel, std::function<void(UnityEngine::Color)> onChange) {
        auto modal = CreateColorPickerModal(parent, "QuestUIColorPickerModal", defaultColor, nullptr, nullptr, nullptr);

        auto colorImageBase = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Image*>(), [](Image* x) { return x->get_gameObject()->get_name() == "ColorImage" && x->get_sprite()->get_name() == "NoteCircle"; });

        FormattedFloatListSettingsValueController* baseSetting = MonoBehaviour::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<FormattedFloatListSettingsValueController*>(), [](auto x) {
            return x->get_name() == "VRRenderingScale";
        }));

        auto go = baseSetting->get_gameObject();
        go->SetActive(false);
        MonoBehaviour::Destroy(baseSetting);

        auto* colorSetting = go->AddComponent<ColorSetting*>();

        auto* valuePick = reinterpret_cast<RectTransform *>(go->get_transform()->Find("ValuePicker"));
        valuePick->set_sizeDelta({13, 0});

        Button* decButton = valuePick->GetComponentsInChildren<Button*>().First();
        decButton->set_enabled(false);
        decButton->set_interactable(true);

        GameObject::Destroy(decButton->get_transform()->Find("Icon")->get_gameObject());
        GameObject::Destroy(valuePick->GetComponentsInChildren<TextMeshProUGUI*>().First()->get_gameObject());
        colorSetting->editButton = valuePick->GetComponentsInChildren<Button*>().Last();

        GameObject* nameText = go->get_transform()->Find("NameText")->get_gameObject();
        auto* _text = nameText->GetComponent<TextMeshProUGUI*>();
        _text->SetText(text);

        auto* externalComponents = go->AddComponent<ExternalComponents*>();
        externalComponents->Add(_text);
        //        externalComponents.Add(localizedText);

        go->GetComponent<LayoutElement*>()->set_preferredWidth(90);

        Image* colorImage = Object::Instantiate(colorImageBase, valuePick, false);
        auto* colorImageRT = reinterpret_cast<RectTransform *>(colorImage->get_gameObject()->get_transform());
        colorImageRT->set_anchoredPosition({0,0});
        colorImageRT->set_sizeDelta({5, 5});
        colorImageRT->set_anchorMin({0.2f, 0.5f});
        colorImageRT->set_anchorMax({0.2f, 0.5f});

        colorSetting->colorImage = colorImage;

        auto* icon = colorSetting->editButton->get_transform()->Find("Icon")->GetComponent<Image*>();
        icon->set_name("EditIcon");
        icon->set_sprite(GetEditIcon());
        icon->get_rectTransform()->set_sizeDelta({4, 4});
        colorSetting->set_interactable(true);

        reinterpret_cast<RectTransform *>(colorSetting->editButton->get_transform())->set_anchorMin({0, 0});

        colorSetting->Setup(modal, defaultColor, std::move(onChange), std::move(onDone), std::move(onCancel));

        go->get_transform()->SetParent(parent, false);

        go->SetActive(true);

        return colorSetting;
    }

    QuestUI::ModalColorPicker* CreateColorPickerModal(UnityEngine::Transform* parent, StringW name, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color)> onDone, std::function<void()> onCancel, std::function<void(UnityEngine::Color)> onChange)
    {
        // base.CreateObject
        auto modal = CreateModal(parent, Vector2(135, 75), Vector2(0, 0), nullptr, false);
        auto gameObject = modal->get_gameObject();

        auto windowTransform = reinterpret_cast<RectTransform*>(gameObject->get_transform());
        
        static ConstString QuestUIModalColorPicker_cs("QuestUIModalColorPicker");
        windowTransform->set_name(QuestUIModalColorPicker_cs);

        auto colorPicker = gameObject->AddComponent<ModalColorPicker*>();
        colorPicker->Setup(defaultColor, onChange, onDone, onCancel);
        colorPicker->modalView = modal;

        static WeakPtrGO<RGBPanelController> rgbTemplate;
        static WeakPtrGO<HSVPanelController> hsvTemplate;
        static WeakPtrGO<ImageView> currentColorTemplate;

        if (!rgbTemplate)
            rgbTemplate = Resources::FindObjectsOfTypeAll<RGBPanelController*>().First([](auto x){ return x->get_name() == "RGBColorPicker"; });

        if (!hsvTemplate)
            hsvTemplate = Resources::FindObjectsOfTypeAll<HSVPanelController*>().First([](auto x){ return x->get_name() == "HSVColorPicker"; });

        if (!currentColorTemplate)
            currentColorTemplate = Resources::FindObjectsOfTypeAll<ImageView*>().First([](auto x){
            if (x->get_name() != "SaberColorA") return false;
            auto parent = x->get_transform()->get_parent();
            if (!parent) return false;
            return parent->get_name() == "ColorSchemeView";
        });

        auto rgbController = Object::Instantiate(rgbTemplate.getInner(), gameObject->get_transform(), false);
        static ConstString QuestUIRGBPanel_cs("QuestUIRGBPanel");
        rgbController->set_name(QuestUIRGBPanel_cs);
        auto rectTransform = reinterpret_cast<RectTransform*>(rgbController->get_transform());
        rectTransform->set_anchoredPosition(Vector2(0, 3));
        rectTransform->set_anchorMin(Vector2(0, 0.25f));
        rectTransform->set_anchorMax(Vector2(0, 0.25f));
        colorPicker->rgbPanel = rgbController;
        
        std::function<void(UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType)> OnChange = std::bind(&ModalColorPicker::OnChange, colorPicker, std::placeholders::_1, std::placeholders::_2);
        auto delegate = custom_types::MakeDelegate<System::Action_2<UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType>*>(classof(System::Action_2<UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType>*), OnChange);
        rgbController->add_colorDidChangeEvent(delegate);

        auto hsvController = Object::Instantiate(hsvTemplate.getInner(), gameObject->get_transform(), false);
        static ConstString QuestUIHSVPanel_cs("QuestUIHSVPanel");
        hsvController->set_name(QuestUIHSVPanel_cs);
        rectTransform = reinterpret_cast<RectTransform*>(hsvController->get_transform());
        rectTransform->set_anchoredPosition(Vector2(0, 3));
        rectTransform->set_anchorMin(Vector2(0.75f, 0.5f));
        rectTransform->set_anchorMax(Vector2(0.75f, 0.5f));
        colorPicker->hsvPanel = hsvController;
        
        hsvController->add_colorDidChangeEvent(delegate);

        auto colorImage = Object::Instantiate(currentColorTemplate.getInner(), gameObject->get_transform(), false);
        static ConstString QuestUICurrentColor_cs("QuestUICurrentColor");
        colorImage->set_name(QuestUICurrentColor_cs);
        rectTransform = reinterpret_cast<RectTransform*>(colorImage->get_transform());
        rectTransform->set_anchoredPosition(Vector2(0, 0));
        rectTransform->set_anchorMin(Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(Vector2(0.5f, 0.5f));
        colorPicker->colorImage = colorImage;

        colorPicker->set_color(defaultColor);

        auto horizontal = CreateHorizontalLayoutGroup(gameObject->get_transform());
        horizontal->get_rectTransform()->set_anchoredPosition(Vector2(0.0, -30));
        horizontal->set_spacing(2);
        horizontal->get_gameObject()->AddComponent<ContentSizeFitter*>()->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);

        auto cancel = CreateUIButton(horizontal->get_transform(), "Cancel", Vector2(0.0, 0.0), Vector2(30, 8), std::bind(&ModalColorPicker::CancelPressed, colorPicker));
        auto done = CreateUIButton(horizontal->get_transform(), "Done", Vector2(0.0, 0.0), Vector2(30, 8), std::bind(&ModalColorPicker::DonePressed, colorPicker));
        
        return colorPicker;
    }

    ModalView* CreateModal(Transform* parent, UnityEngine::Vector2 sizeDelta, UnityEngine::Vector2 anchoredPosition, std::function<void(ModalView*)> onBlockerClicked, bool dismissOnBlockerClicked) {
        static ConstString name("QuestUIModalPrefab");

        // declare var
        ModalView* orig = modalPrefab->GetComponent<ModalView*>();
        
        // instantiate
        GameObject* modalObj = Object::Instantiate(modalPrefab, parent, false);
        
        modalObj->set_name(name);
        modalObj->SetActive(false);

        // get the modal
        ModalView* modal = modalObj->GetComponent<ModalView*>();

        // copy fields
        modal->presentPanelAnimations = orig->presentPanelAnimations;
        modal->dismissPanelAnimation = orig->dismissPanelAnimation;
        modal->container = GetDiContainer();
        modalObj->GetComponent<VRGraphicRaycaster*>()->physicsRaycaster = GetPhysicsRaycasterWithCache();

        // destroy unneeded objects
        Object::DestroyImmediate(modalObj->GetComponent<TableView*>());
        Object::DestroyImmediate(modalObj->GetComponent<ScrollRect*>());
        Object::DestroyImmediate(modalObj->GetComponent<ScrollView*>());
        Object::DestroyImmediate(modalObj->GetComponent<EventSystemListener*>());

        // destroy all children except background
        int childCount = modal->get_transform()->get_childCount();
        for (int i = 0; i < childCount; i++) {
            auto* child = modal->get_transform()->GetChild(i)->GetComponent<RectTransform*>();

            if (child->get_gameObject()->get_name() == "BG") {
                child->set_anchoredPosition(Vector2::get_zero());
                child->set_sizeDelta(Vector2::get_zero());
                child->GetComponent<Image*>()->set_raycastTarget(true);
            }
            else {
                // yeet the child
                Object::Destroy(child->get_gameObject());
            }
        }

        // set recttransform data
        auto rect = modalObj->GetComponent<RectTransform*>();
        rect->set_anchorMin({0.5f, 0.5f});
        rect->set_anchorMax({0.5f, 0.5f});
        rect->set_sizeDelta(sizeDelta);
        rect->set_anchoredPosition(anchoredPosition);

        // add callback
        modal->add_blockerClickedEvent(MakeDelegate(System::Action*,
            ((std::function<void()>) [onBlockerClicked, modal, dismissOnBlockerClicked] () {
                if (onBlockerClicked)
                    onBlockerClicked(modal); 
                if (dismissOnBlockerClicked)
                    modal->Hide(true, nullptr);
            })
        ));
        return modal;
    }

    ModalView* CreateModal(Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(ModalView*)> onBlockerClicked, bool dismissOnBlockerClicked) {
        return CreateModal(parent, sizeDelta, {0.0f, 0.0f}, onBlockerClicked, dismissOnBlockerClicked);
    }

    ModalView* CreateModal(Transform* parent, std::function<void(ModalView*)> onBlockerClicked, bool dismissOnBlockerClicked) {
        return CreateModal(parent, {30.0f, 40.0f}, {0.0f, 0.0f}, onBlockerClicked, dismissOnBlockerClicked);
    }

    ModalView* CreateModal(Transform* parent, bool dismissOnBlockerClicked) {
        return CreateModal(parent, {30.0f, 40.0f}, {0.0f, 0.0f}, nullptr, dismissOnBlockerClicked);
    }

    GameObject* CreateScrollableModalContainer(ModalView* modal) {
        Vector2 sizeDelta = modal->GetComponent<RectTransform*>()->get_sizeDelta();
        float width = sizeDelta.x;
        float height = sizeDelta.y;

        Transform* parent = modal->get_transform();
        GameObject* content = CreateScrollView(parent);

        ExternalComponents* externalComponents = content->GetComponent<ExternalComponents*>();
        RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();

        scrollTransform->set_anchoredPosition({-2.5f, 0.0f});
        scrollTransform->set_sizeDelta({7.5f, 0.0f});

        VerticalLayoutGroup* layout = content->GetComponent<VerticalLayoutGroup*>();
        
        layout->set_childControlWidth(true);
        layout->set_childForceExpandWidth(true);

        auto layoutelem = content->AddComponent<LayoutElement*>();
        layoutelem->set_preferredWidth(width - 10.0f);

        static ConstString name("QuestUICreateScrollableModalContainer");

        scrollTransform->get_gameObject()->set_name(name);
        return content;
    }
    
    // not used anymore, but might be useful to keep around?
    /*
    Button* CreatePageButton(StringW name, Transform* parent, std::string_view buttonTemplate, Vector2 anchoredPosition, Vector2 sizeDelta, std::function<void(void)> onClick, Sprite* icon = nullptr)
    {
        StringW templCS(buttonTemplate);
        auto orig = Resources::FindObjectsOfTypeAll<Button*>().Last([&](auto x) {
            return x->get_name()->Equals(templCS);
        });

        if (!icon)
            icon = orig->GetComponentInChildren<Image*>()->get_sprite();
        
        auto btn = Object::Instantiate(orig, parent, false);
        btn->GetComponentInChildren<Image*>()->set_sprite(icon);
        btn->set_name(name);
        btn->set_interactable(true);
        auto iconrect = btn->GetComponentInChildren<Image*>()->get_rectTransform();
        
        auto rect = reinterpret_cast<RectTransform*>(btn->get_transform());
        rect->set_anchorMin(Vector2(0.0f, 0.0f));
        rect->set_anchorMax(Vector2(1.0f, 1.0f));
        rect->set_anchoredPosition(anchoredPosition);
        rect->set_sizeDelta(sizeDelta);
        rect->set_pivot(Vector2(0.5f, 0.5f));

        SetButtonText(btn, "");

        btn->set_onClick(Button::ButtonClickedEvent::New_ctor());

        if (onClick)
            btn->get_onClick()->AddListener(MakeDelegate(UnityAction*, onClick));

        return btn;
    }
    */
    
    CustomListTableData* CreateList(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateList(parent, {35.0f, 60.0f}, onCellWithIdxClicked);
    }

    CustomListTableData* CreateList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateList(parent, {0.0f, 0.0f}, sizeDelta, onCellWithIdxClicked);
    }

    // BSML ListTag
    CustomListTableData* CreateList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked)
    {
        static ConstString QuestUIListContainer_cs("QuestUIListContainer");
        auto container = GameObject::New_ctor(QuestUIListContainer_cs)->AddComponent<RectTransform*>();
        container->set_anchoredPosition(anchoredPosition);
        container->set_sizeDelta(sizeDelta);
        auto layoutElement = container->get_gameObject()->AddComponent<LayoutElement*>();
        layoutElement->set_flexibleHeight(sizeDelta.y);
        layoutElement->set_minHeight(sizeDelta.y);
        layoutElement->set_preferredHeight(sizeDelta.y);
        layoutElement->set_flexibleHeight(sizeDelta.x);
        layoutElement->set_minHeight(sizeDelta.x);
        layoutElement->set_preferredHeight(sizeDelta.x);

        container->SetParent(parent, false);

        static ConstString QuestUIList_cs("QuestUIList");
        auto gameObject = GameObject::New_ctor(QuestUIList_cs);
        gameObject->get_transform()->SetParent(container, false);
        gameObject->SetActive(false);

        auto canvasTemplate = getCanvasTemplate();

        gameObject->AddComponent<ScrollRect*>();
        auto canvas = gameObject->AddComponent<Canvas*>();
        // copy the template canvas
        canvas->set_additionalShaderChannels(canvasTemplate->get_additionalShaderChannels());
        canvas->set_overrideSorting(canvasTemplate->get_overrideSorting());
        canvas->set_pixelPerfect(canvasTemplate->get_pixelPerfect());
        canvas->set_referencePixelsPerUnit(canvasTemplate->get_referencePixelsPerUnit());
        canvas->set_renderMode(canvasTemplate->get_renderMode());
        canvas->set_scaleFactor(canvasTemplate->get_scaleFactor());
        canvas->set_sortingLayerID(canvasTemplate->get_sortingLayerID());
        canvas->set_sortingOrder(canvasTemplate->get_sortingOrder());
        canvas->set_worldCamera(canvasTemplate->get_worldCamera());

        gameObject->AddComponent<VRGraphicRaycaster*>()->physicsRaycaster = GetPhysicsRaycasterWithCache();
        gameObject->AddComponent<Touchable*>();
        gameObject->AddComponent<EventSystemListener*>();
        auto scrollView = gameObject->AddComponent<ScrollView*>();

        HMUI::TableView* tableView = gameObject->AddComponent<QuestUI::TableView*>();
        auto tableData = container->get_gameObject()->AddComponent<CustomListTableData*>();
        tableData->tableView = tableView;
        
        tableView->preallocatedCells = Array<TableView::CellsGroup*>::NewLength(0);
        tableView->isInitialized = false;
        tableView->scrollView = scrollView;

        static ConstString Viewport_cs("Viewport");
        auto viewport = GameObject::New_ctor(Viewport_cs)->AddComponent<RectTransform*>();
        viewport->SetParent(gameObject->GetComponent<RectTransform*>(), false);
        viewport->get_gameObject()->AddComponent<RectMask2D*>();
        gameObject->GetComponent<ScrollRect*>()->set_viewport(viewport);

        static ConstString Content_cs("Content");
        auto content = GameObject::New_ctor(Content_cs)->AddComponent<RectTransform*>();
        content->SetParent(viewport, false);

        scrollView->contentRectTransform = content;
        scrollView->viewport = viewport;

        viewport->set_anchorMin({0.0f, 0.0f});
        viewport->set_anchorMax({1.0f, 1.0f});
        viewport->set_sizeDelta({0.0f, 0.0f});
        viewport->set_anchoredPosition({0.0f, 0.0f});
        
        auto tableViewRectT = tableView->GetComponent<RectTransform*>();
        tableViewRectT->set_anchorMin({0.0f, 0.0f});
        tableViewRectT->set_anchorMax({1.0f, 1.0f});
        tableViewRectT->set_sizeDelta({0.0f, 0.0f});
        tableViewRectT->set_anchoredPosition({0.0f, 0.0f});

        // reinterpret cast because interfaces are not explicitly inherited
        tableView->SetDataSource(reinterpret_cast<HMUI::TableView::IDataSource*>(tableData), false);

        if (onCellWithIdxClicked)
        {
            using DelegateType = System::Action_2<HMUI::TableView*, int>;
            std::function<void(HMUI::TableView*, int)> fun = [onCellWithIdxClicked](HMUI::TableView*, int idx){
                onCellWithIdxClicked(idx);
            };
            auto delegate = custom_types::MakeDelegate<DelegateType*>(classof(DelegateType*), fun);
            tableView->add_didSelectCellWithIdxEvent(delegate);
        }

        tableView->get_gameObject()->SetActive(true);
        return tableData;
    }

    CustomListTableData* CreateScrollableList(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateScrollableList(parent, {35.0f, 60.0f}, onCellWithIdxClicked);
    }

    CustomListTableData* CreateScrollableList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateScrollableList(parent, {0.0f, 0.0f}, sizeDelta, onCellWithIdxClicked);
    }

    CustomListTableData* CreateScrollableList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked)
    {
        auto vertical = CreateVerticalLayoutGroup(parent);
        auto layout = vertical->get_gameObject()->AddComponent<LayoutElement*>(); 
        vertical->get_rectTransform()->set_sizeDelta(sizeDelta);
        vertical->get_rectTransform()->set_anchoredPosition(anchoredPosition);
        vertical->set_childForceExpandHeight(false);
        vertical->set_childControlHeight(false);
        vertical->set_childScaleHeight(false);
        layout->set_preferredHeight(sizeDelta.y);
        layout->set_preferredWidth(sizeDelta.x);

        auto list = CreateList(vertical->get_transform(), Vector2 (sizeDelta.x, sizeDelta.y - 16.0f), onCellWithIdxClicked);
        auto rect = list->GetComponent<RectTransform*>();

        auto up = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [list](){ 
            int idx = reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrolledRow();
            idx -= reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrollDistance();
            idx = idx > 0 ? idx : 0;
            list->tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
        });
        up->get_transform()->SetAsFirstSibling();

        auto down = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [list](){
            int idx = reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrolledRow();
            idx += reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrollDistance();
            int max = list->tableView->get_dataSource()->NumberOfCells();
            idx = max <= idx ? max - 1 : idx;
            list->tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
        });
        down->get_transform()->SetAsLastSibling();
        
        // replace sprites
        reinterpret_cast<RectTransform*>(up->get_transform()->GetChild(0))->set_sizeDelta({8.0f, 8.0f});
        reinterpret_cast<RectTransform*>(down->get_transform()->GetChild(0))->set_sizeDelta({8.0f, 8.0f});
        auto carat_up_sprite = Base64ToSprite(carat_up);
        auto carat_down_sprite = Base64ToSprite(carat_down);
        auto carat_up_inactive_sprite = Base64ToSprite(carat_up_inactive);
        auto carat_down_inactive_sprite = Base64ToSprite(carat_down_inactive);
        SetButtonSprites(up, carat_up_inactive_sprite, carat_up_sprite);
        SetButtonSprites(down, carat_down_inactive_sprite, carat_down_sprite);

        return list;
    }

    HMUI::TableView::IDataSource* CreateCustomSourceList(System::Type* type, UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked)
    {
        static ConstString QuestUIListContainer_cs("QuestUIListContainer");
        auto container = GameObject::New_ctor(QuestUIListContainer_cs)->AddComponent<RectTransform*>();
        container->set_anchoredPosition(anchoredPosition);
        container->set_sizeDelta(sizeDelta);
        auto layoutElement = container->get_gameObject()->AddComponent<LayoutElement*>();
        layoutElement->set_flexibleHeight(sizeDelta.y);
        layoutElement->set_minHeight(sizeDelta.y);
        layoutElement->set_preferredHeight(sizeDelta.y);
        layoutElement->set_flexibleHeight(sizeDelta.x);
        layoutElement->set_minHeight(sizeDelta.x);
        layoutElement->set_preferredHeight(sizeDelta.x);

        container->SetParent(parent, false);

        static ConstString QuestUIList_cs("QuestUIList");
        auto gameObject = GameObject::New_ctor(QuestUIList_cs);
        gameObject->get_transform()->SetParent(container, false);
        gameObject->SetActive(false);

        auto canvasTemplate = getCanvasTemplate();

        gameObject->AddComponent<ScrollRect*>();
        auto canvas = gameObject->AddComponent<Canvas*>();
        // copy the template canvas
        canvas->set_additionalShaderChannels(canvasTemplate->get_additionalShaderChannels());
        canvas->set_overrideSorting(canvasTemplate->get_overrideSorting());
        canvas->set_pixelPerfect(canvasTemplate->get_pixelPerfect());
        canvas->set_referencePixelsPerUnit(canvasTemplate->get_referencePixelsPerUnit());
        canvas->set_renderMode(canvasTemplate->get_renderMode());
        canvas->set_scaleFactor(canvasTemplate->get_scaleFactor());
        canvas->set_sortingLayerID(canvasTemplate->get_sortingLayerID());
        canvas->set_sortingOrder(canvasTemplate->get_sortingOrder());
        canvas->set_worldCamera(canvasTemplate->get_worldCamera());

        gameObject->AddComponent<VRGraphicRaycaster*>()->physicsRaycaster = GetPhysicsRaycasterWithCache();
        gameObject->AddComponent<Touchable*>();
        gameObject->AddComponent<EventSystemListener*>();
        auto scrollView = gameObject->AddComponent<ScrollView*>();

        HMUI::TableView* tableView = gameObject->AddComponent<QuestUI::TableView*>();
        HMUI::TableView::IDataSource* tableData = reinterpret_cast<HMUI::TableView::IDataSource*>(container->get_gameObject()->AddComponent(type));
        il2cpp_utils::SetFieldValue(tableData, "tableView", tableView);
        
        tableView->preallocatedCells = Array<TableView::CellsGroup*>::NewLength(0);
        tableView->isInitialized = false;
        tableView->scrollView = scrollView;

        static ConstString Viewport_cs("Viewport");
        auto viewport = GameObject::New_ctor(Viewport_cs)->AddComponent<RectTransform*>();
        viewport->SetParent(gameObject->GetComponent<RectTransform*>(), false);
        viewport->get_gameObject()->AddComponent<RectMask2D*>();
        gameObject->GetComponent<ScrollRect*>()->set_viewport(viewport);

        static ConstString Content_cs("Content");
        auto content = GameObject::New_ctor(Content_cs)->AddComponent<RectTransform*>();
        content->SetParent(viewport, false);

        scrollView->contentRectTransform = content;
        scrollView->viewport = viewport;

        viewport->set_anchorMin({0.0f, 0.0f});
        viewport->set_anchorMax({1.0f, 1.0f});
        viewport->set_sizeDelta({0.0f, 0.0f});
        viewport->set_anchoredPosition({0.0f, 0.0f});
        
        auto tableViewRectT = tableView->GetComponent<RectTransform*>();
        tableViewRectT->set_anchorMin({0.0f, 0.0f});
        tableViewRectT->set_anchorMax({1.0f, 1.0f});
        tableViewRectT->set_sizeDelta({0.0f, 0.0f});
        tableViewRectT->set_anchoredPosition({0.0f, 0.0f});

        // reinterpret cast because interfaces are not explicitly inherited
        tableView->SetDataSource(tableData, false);

        if (onCellWithIdxClicked)
        {
            using DelegateType = System::Action_2<HMUI::TableView*, int>;
            std::function<void(HMUI::TableView*, int)> fun = [onCellWithIdxClicked](HMUI::TableView*, int idx){
                onCellWithIdxClicked(idx);
            };
            auto delegate = custom_types::MakeDelegate<DelegateType*>(classof(DelegateType*), fun);
            tableView->add_didSelectCellWithIdxEvent(delegate);
        }

        tableView->get_gameObject()->SetActive(true);
        return tableData;
    }

    HMUI::TableView::IDataSource* CreateScrollableCustomSourceList(System::Type* type, UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked)
    {
        auto vertical = CreateVerticalLayoutGroup(parent);
        auto layout = vertical->get_gameObject()->AddComponent<LayoutElement*>(); 
        vertical->get_rectTransform()->set_sizeDelta(sizeDelta);
        vertical->get_rectTransform()->set_anchoredPosition(anchoredPosition);
        vertical->set_childForceExpandHeight(false);
        vertical->set_childControlHeight(false);
        vertical->set_childScaleHeight(false);
        layout->set_preferredHeight(sizeDelta.y);
        layout->set_preferredWidth(sizeDelta.x);

        HMUI::TableView::IDataSource* list = CreateCustomSourceList(type, vertical->get_transform(), Vector2(0.0f, 0.0f), Vector2 (sizeDelta.x, sizeDelta.y - 16.0f), onCellWithIdxClicked);
        auto rect = reinterpret_cast<UnityEngine::MonoBehaviour*>(list)->GetComponent<RectTransform*>();

        auto up = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [list](){ 
            auto tableView = CRASH_UNLESS(il2cpp_utils::GetFieldValue<QuestUI::TableView*>(reinterpret_cast<Il2CppObject*>(list), "tableView"));
            int idx = tableView->get_scrolledRow();
            idx -= tableView->get_scrollDistance();
            idx = idx > 0 ? idx : 0;
            tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
        });
        up->get_transform()->SetAsFirstSibling();

        auto down = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [list](){
            auto tableView = CRASH_UNLESS(il2cpp_utils::GetFieldValue<QuestUI::TableView*>(reinterpret_cast<Il2CppObject*>(list), "tableView"));
            int idx = tableView->get_scrolledRow();
            idx += tableView->get_scrollDistance();
            int max = tableView->get_dataSource()->NumberOfCells();
            idx = max <= idx ? max - 1 : idx;
            tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
        });
        down->get_transform()->SetAsLastSibling();
        
        // replace sprites
        reinterpret_cast<RectTransform*>(up->get_transform()->GetChild(0))->set_sizeDelta({8.0f, 8.0f});
        reinterpret_cast<RectTransform*>(down->get_transform()->GetChild(0))->set_sizeDelta({8.0f, 8.0f});
        auto carat_up_sprite = Base64ToSprite(carat_up);
        auto carat_down_sprite = Base64ToSprite(carat_down);
        auto carat_up_inactive_sprite = Base64ToSprite(carat_up_inactive);
        auto carat_down_inactive_sprite = Base64ToSprite(carat_down_inactive);
        SetButtonSprites(up, carat_up_inactive_sprite, carat_up_sprite);
        SetButtonSprites(down, carat_down_inactive_sprite, carat_down_sprite);

        return list;
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, ArrayW<StringW> values, std::function<void(int)> onCellWithIdxClicked)
    {
        static WeakPtrGO<TextSegmentedControl> segmentedControlTemplate;
        if (!segmentedControlTemplate)
            segmentedControlTemplate = Resources::FindObjectsOfTypeAll<HMUI::TextSegmentedControl*>().First([](auto x){
            if (x->get_name() != "TextSegmentedControl") return false;
            auto parent = x->get_transform()->get_parent();
            if (!parent) return false;
            auto parentName = parent->get_name();
            return parentName == "GameplaySetupViewController" || parentName == "BaseGameplaySetupWrapper";
        });

        auto segmentedControlObj = Object::Instantiate(segmentedControlTemplate->get_gameObject(), parent, false);
        segmentedControlObj->SetActive(false);
        static ConstString QuestUITextSegmentedControl("QuestUITextSegmentedControl");
        segmentedControlObj->set_name(QuestUITextSegmentedControl);
        auto rectTransform = reinterpret_cast<RectTransform*>(segmentedControlObj->get_transform());
        rectTransform->set_sizeDelta(sizeDelta);
        rectTransform->set_anchoredPosition(anchoredPosition);


        Object::DestroyImmediate(segmentedControlObj->GetComponent<HMUI::TextSegmentedControl*>());
        auto control = segmentedControlObj->AddComponent<HMUI::SegmentedControl*>();
        auto result = segmentedControlObj->AddComponent<QuestUI::CustomTextSegmentedControlData*>();
        
        result->firstCellPrefab = segmentedControlTemplate->firstCellPrefab;
        result->lastCellPrefab = segmentedControlTemplate->lastCellPrefab;
        result->middleCellPrefab = segmentedControlTemplate->middleCellPrefab;
        result->singleCellPrefab = segmentedControlTemplate->singleCellPrefab;

        result->segmentedControl = control;
        control->dataSource = reinterpret_cast<HMUI::SegmentedControl::IDataSource*>(result);
        
        if (onCellWithIdxClicked)
        {
            std::function<void(HMUI::SegmentedControl*, int)> fun = [onCellWithIdxClicked](HMUI::SegmentedControl* cell, int idx){ onCellWithIdxClicked(idx); };
            auto delegate = custom_types::MakeDelegate<System::Action_2<HMUI::SegmentedControl*, int>*>(classof(System::Action_2<HMUI::SegmentedControl*, int>*), fun);
            control->add_didSelectCellEvent(delegate);
        }

        int childCount = result->get_transform()->get_childCount();
        for (int i = 0; i < childCount; i++) 
        {
            Object::DestroyImmediate(result->get_transform()->GetChild(0)->get_gameObject());
        }
        
        result->set_texts(values);

        segmentedControlObj->SetActive(true);
        return result;
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, ArrayW<StringW> values, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, sizeDelta, values, onCellWithIdxClicked);
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, ArrayW<StringW> values, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, {80, 10}, values, onCellWithIdxClicked);
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, {80, 10}, ArrayW<StringW>(static_cast<il2cpp_array_size_t>(0)), onCellWithIdxClicked);
    }

    QuestUI::ProgressBar* CreateProgressBar(Vector3 const& position, StringW headerText, StringW subText1, StringW subText2){
        return CreateProgressBar(position, {0,0,0}, {1,1,1}, headerText, subText1, subText2);
    }
    QuestUI::ProgressBar* CreateProgressBar(Vector3 const& position, Vector3 const& rotation, StringW headerText, StringW subText1, StringW subText2){
        return CreateProgressBar(position, rotation, {1,1,1}, headerText, subText1, subText2);
    }

    QuestUI::ProgressBar* CreateProgressBar(Vector3 const& position, Vector3 const& rotation, Vector3 const& scale, StringW headerText, StringW subText1, StringW subText2){
        auto bar = UnityEngine::GameObject::New_ctor("LoadingStatus")->AddComponent<ProgressBar*>();
        auto barGameObject = bar->get_gameObject();
        auto barObjectTransform = bar->get_transform();
        barObjectTransform->set_position(position);
        barObjectTransform->set_eulerAngles(rotation);
        barObjectTransform->set_localScale(scale / 100);

        bar->canvas = barGameObject->AddComponent<UnityEngine::Canvas*>();
        bar->canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);
        barGameObject->AddComponent<HMUI::CurvedCanvasSettings*>()->SetRadius(0.0f);

        auto ct = bar->canvas->get_transform();
        ct->set_position(position);
        ct->set_localScale(scale / 100);

        Vector2 LoadingBarSize = {100, 10};
        Color BackgroundColor = {0, 0, 0, 0.2f};
        auto rectTransform = reinterpret_cast<UnityEngine::RectTransform*>(ct);
        rectTransform->set_sizeDelta({200, 50});

        // why set everything after creating it in the first place ?
        bar->subText2 = CreateText(ct, subText2, false, {10, 31}, {100, 20});
        bar->subText2->set_fontSize(7.0f);
        bar->subText1 = CreateText(ct, subText1, false, {10, 23}, {100, 20});
        bar->subText1->set_fontSize(9.0f);
        bar->headerText = CreateText(ct, headerText, false,{10, 15}, {100, 20});
        bar->headerText->set_fontSize(15.0f);

        bar->loadingBackground = UnityEngine::GameObject::New_ctor("Background")->AddComponent<UnityEngine::UI::Image*>();
        rectTransform = reinterpret_cast<UnityEngine::RectTransform*>(bar->loadingBackground->get_transform());
        rectTransform->SetParent(ct, false); 
        rectTransform->set_sizeDelta(LoadingBarSize);
        bar->loadingBackground->set_color(BackgroundColor);
        
        bar->loadingBar = UnityEngine::GameObject::New_ctor("Loading Bar")->AddComponent<UnityEngine::UI::Image*>();
        rectTransform = reinterpret_cast<UnityEngine::RectTransform*>(bar->loadingBar->get_transform());
        rectTransform->SetParent(ct, false);
        rectTransform->set_sizeDelta(LoadingBarSize);

        auto tex = UnityEngine::Texture2D::get_whiteTexture();
        auto sprite = UnityEngine::Sprite::CreateSprite(tex, UnityEngine::Rect(0, 0, tex->get_width(), tex->get_height()), {0.5f, 0.5f}, 100.0f, 1, UnityEngine::SpriteMeshType::Tight, UnityEngine::Vector4::get_zero(), false);
        bar->loadingBar->set_sprite(sprite);
        bar->loadingBar->set_type(UnityEngine::UI::Image::Type::Filled);
        bar->loadingBar->set_fillMethod(UnityEngine::UI::Image::FillMethod::Horizontal);
        bar->loadingBar->set_color({1.0f, 1.0f, 1.0f, 0.5f});
        
        UnityEngine::Object::DontDestroyOnLoad(bar->get_gameObject());
        bar->inited = true;
        return bar;
    }
}
