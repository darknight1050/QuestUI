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

#include "GlobalNamespace/UIKeyboardManager.hpp"
#include "GlobalNamespace/BoolSettingsController.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
#include "GlobalNamespace/ReleaseInfoViewController.hpp"
#include "GlobalNamespace/ColorPickerButtonController.hpp"
#include "GlobalNamespace/HSVPanelController.hpp"

#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/CanvasGroup.hpp"
#include "UnityEngine/AdditionalCanvasShaderChannels.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Rect.hpp"
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

#include "VRUIControls/VRGraphicRaycaster.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"

#include "System/Convert.hpp"
#include "System/Action_2.hpp"
#include "System/Action.hpp"
#include "System/Collections/Generic/HashSet_1.hpp"

#include "Zenject/DiContainer.hpp"

#include "customlogger.hpp"
#include "Sprites/carats.hpp"
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

#define MakeDelegate(DelegateType, varName) (il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

namespace QuestUI::BeatSaberUI {

    GameObject* beatSaberUIObject = nullptr;
    GameObject* dropdownListPrefab = nullptr;
    GameObject* modalPrefab = nullptr;
    
    void SetupPersistentObjects() {
        getLogger().info("SetupPersistentObjects");
        if(!beatSaberUIObject) {
            static auto name = il2cpp_utils::createcsstr("BeatSaberUIObject", il2cpp_utils::StringType::Manual);
            beatSaberUIObject = GameObject::New_ctor(name);
            GameObject::DontDestroyOnLoad(beatSaberUIObject);
            beatSaberUIObject->AddComponent<MainThreadScheduler*>();
        }
        if(!dropdownListPrefab) {
            GameObject* search = ArrayUtil::First(Resources::FindObjectsOfTypeAll<SimpleTextDropdown*>(), [](SimpleTextDropdown* x) { 
                    return to_utf8(csstrtostr(x->get_transform()->get_parent()->get_name())) == "NormalLevels"; 
                }
            )->get_transform()->get_parent()->get_gameObject();
            dropdownListPrefab = Object::Instantiate(search, beatSaberUIObject->get_transform(), false);
            static auto name = il2cpp_utils::createcsstr("QuestUIDropdownListPrefab", il2cpp_utils::StringType::Manual);
            dropdownListPrefab->set_name(name);
            dropdownListPrefab->SetActive(false);
        }
        if (!modalPrefab) {
            GameObject* search = ArrayUtil::First(Resources::FindObjectsOfTypeAll<ModalView*>(), [](ModalView* x) { 
                    return to_utf8(csstrtostr(x->get_transform()->get_name())) == "DropdownTableView";
                }
            )->get_gameObject();
            modalPrefab = Object::Instantiate(search, beatSaberUIObject->get_transform(), false);
            
            modalPrefab->GetComponent<ModalView*>()->presentPanelAnimations = search->GetComponent<ModalView*>()->presentPanelAnimations;
            modalPrefab->GetComponent<ModalView*>()->dismissPanelAnimation = search->GetComponent<ModalView*>()->dismissPanelAnimation;

            static auto name = il2cpp_utils::createcsstr("QuestUIModalPrefab", il2cpp_utils::StringType::Manual);
            modalPrefab->set_name(name);
            modalPrefab->SetActive(false);
        }
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
            mainTextFont = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TMP_FontAsset*>(), [](TMP_FontAsset* x) { return to_utf8(csstrtostr(x->get_name())) == "Teko-Medium SDF"; });
        if(!mainTextFont)
            CacheNotFoundWarningLog(MainTextFont);
        return mainTextFont;
    }

    Material* mainUIFontMaterial = nullptr;
    Material* GetMainUIFontMaterial()
    {
        if(!mainUIFontMaterial)
            mainUIFontMaterial = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Material*>(), [](Material* x) { return to_utf8(csstrtostr(x->get_name())) == "Teko-Medium SDF Curved Softer"; });
        if(!mainUIFontMaterial)
            CacheNotFoundWarningLog(MainUIFontMaterial);
        return mainUIFontMaterial;
    }

    Sprite* editIcon = nullptr;
    Sprite* GetEditIcon() {
        if(!editIcon)
            editIcon = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Image*>(), [](Image* x) { return x->get_sprite() && to_utf8(csstrtostr(x->get_sprite()->get_name())) == "EditIcon"; })->get_sprite();
        if(!editIcon)
            CacheNotFoundWarningLog(EditIcon);
        return editIcon;
    }

    PhysicsRaycasterWithCache* physicsRaycaster = nullptr;
    PhysicsRaycasterWithCache* GetPhysicsRaycasterWithCache()
    {
        if(!physicsRaycaster)
            physicsRaycaster = ArrayUtil::First(Resources::FindObjectsOfTypeAll<MainMenuViewController*>())->GetComponent<VRGraphicRaycaster*>()->physicsRaycaster;
        if(!physicsRaycaster)
            CacheNotFoundWarningLog(PhysicsRaycasterWithCache);
        return physicsRaycaster;
    }

    DiContainer* diContainer = nullptr;
    DiContainer* GetDiContainer()
    {
        if(!diContainer)
            diContainer = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TextSegmentedControl*>(), [](TextSegmentedControl* x) { return to_utf8(csstrtostr(x->get_transform()->get_parent()->get_name())) == "PlayerStatisticsViewController" && x->container; })->container;
        if(!diContainer)
            CacheNotFoundWarningLog(DiContainer);
        return diContainer;
    }

    void ClearCache() {
        mainFlowCoordinator = nullptr;
        mainTextFont = nullptr;
        mainUIFontMaterial = nullptr;
        editIcon = nullptr;
        physicsRaycaster = nullptr;
        diContainer = nullptr;
    }

    GameObject* CreateCanvas() {
        static auto name = il2cpp_utils::createcsstr("QuestUICanvas", il2cpp_utils::StringType::Manual);
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
        static auto name = il2cpp_utils::createcsstr("QuestUIFlowCoordinator", il2cpp_utils::StringType::Manual);
        FlowCoordinator* flowCoordinator = reinterpret_cast<FlowCoordinator*>(GameObject::New_ctor(name)->AddComponent(type));
        flowCoordinator->baseInputModule = GetMainFlowCoordinator()->baseInputModule;
        return flowCoordinator;
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, true, anchoredPosition);
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        return CreateText(parent, text, true, anchoredPosition, sizeDelta);
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, bool italic) {
        return CreateText(parent, text, italic, UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, bool italic, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, italic, anchoredPosition, UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static auto name = il2cpp_utils::createcsstr("QuestUIText", il2cpp_utils::StringType::Manual);
        GameObject* gameObj = GameObject::New_ctor(name);
        gameObj->SetActive(false);

        CurvedTextMeshPro* textMesh = gameObj->AddComponent<CurvedTextMeshPro*>();
        RectTransform* rectTransform = textMesh->get_rectTransform();
        rectTransform->SetParent(parent, false);
        textMesh->set_font(GetMainTextFont());
        textMesh->set_fontSharedMaterial(GetMainUIFontMaterial());
        textMesh->set_text(il2cpp_utils::createcsstr(italic ? string_format("<i>%s</i>", text.data()) : text));
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

    Button* CreateUIButton(Transform* parent, std::string buttonText, std::string buttonTemplate, std::function<void()> onClick) {
        Button* button = Object::Instantiate(ArrayUtil::Last(Resources::FindObjectsOfTypeAll<Button*>(), [&buttonTemplate](Button* x) { return to_utf8(csstrtostr(x->get_name())) == buttonTemplate; }), parent, false);
        button->set_onClick(Button::ButtonClickedEvent::New_ctor());
        static auto name = il2cpp_utils::createcsstr("QuestUIButton", il2cpp_utils::StringType::Manual);
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

        SetButtonText(button, buttonText);

        HorizontalLayoutGroup* horiztonalLayoutGroup = button->GetComponentInChildren<HorizontalLayoutGroup*>();
        if (horiztonalLayoutGroup != nullptr)
            externalComponents->Add(horiztonalLayoutGroup);
        
        // if the original button was for some reason not interactable, now it will be
        button->set_interactable(true);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick) {
        Button* button = CreateUIButton(parent, buttonText, buttonTemplate, onClick);
        button->GetComponent<RectTransform*>()->set_anchoredPosition(anchoredPosition);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick) {
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

    Button* CreateUIButton(Transform* parent, std::string buttonText, std::function<void()> onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, onClick);
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, anchoredPosition, onClick);
    }

    Button* CreateUIButton(Transform* parent, std::string buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick) {
        return CreateUIButton(parent, buttonText, DEFAULT_BUTTONTEMPLATE, anchoredPosition, sizeDelta, onClick);
    }

    ImageView* CreateImage(Transform* parent, Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        static auto name = il2cpp_utils::createcsstr("QuestUIImage", il2cpp_utils::StringType::Manual);
        GameObject* gameObj = GameObject::New_ctor(name);
        auto mat_UINoGlow = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Material*>(), [](Material* x) { return to_utf8(csstrtostr(x->get_name())) == "UINoGlow"; });
        ImageView* image = gameObj->AddComponent<ImageView*>();
        image->set_material(mat_UINoGlow);
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

    Sprite* Base64ToSprite(std::string& base64, int width, int height)
    {
        return Base64ToSprite(base64);
    }

    Sprite* Base64ToSprite(std::string& base64)
    {
        Array<uint8_t>* bytes = System::Convert::FromBase64String(il2cpp_utils::createcsstr(base64));
        return ArrayToSprite(bytes);
    }

    Sprite* FileToSprite(std::string& filePath, int width, int height)
    {
        return FileToSprite(filePath);
    }

    Sprite* FileToSprite(std::string& filePath)
    {
        std::ifstream instream(filePath, std::ios::in | std::ios::binary);
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
        Array<uint8_t>* bytes = il2cpp_utils::vectorToArray(data);
        return ArrayToSprite(bytes);
    }

    Sprite* ArrayToSprite(Array<uint8_t>* bytes)
    {
        Texture2D* texture = Texture2D::New_ctor(0, 0, TextureFormat::RGBA32, false, false);
        if (ImageConversion::LoadImage(texture, bytes, false)) {
            texture->set_wrapMode(TextureWrapMode::Clamp);
            return Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), UnityEngine::Vector2(0.5f,0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, UnityEngine::Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        }
        return nullptr;
    }

    GridLayoutGroup* CreateGridLayoutGroup(Transform* parent) {
        static auto name = il2cpp_utils::createcsstr("QuestUIGridLayoutGroup", il2cpp_utils::StringType::Manual);
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
        static auto name = il2cpp_utils::createcsstr("QuestUIHorizontalLayoutGroup", il2cpp_utils::StringType::Manual);
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
        static auto name = il2cpp_utils::createcsstr("QuestUIVerticalLayoutGroup", il2cpp_utils::StringType::Manual);
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
    Toggle* CreateToggle(Transform* parent, std::string text, std::function<void(bool)> onValueChange)
    {
        return CreateToggle(parent, text, false, onValueChange);
    }

    Toggle* CreateToggle(Transform* parent, std::string text, bool currentValue, std::function<void(bool)> onValueChange)
    {
        return CreateToggle(parent, text, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    Toggle* CreateToggle(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onValueChange)
    {
        return CreateToggle(parent, text, false, anchoredPosition, onValueChange);
    }
    
    Toggle* CreateToggle(Transform* parent, std::string text, bool currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onValueChange)
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
        toggle->set_isOn(currentValue);
        toggle->onValueChanged = Toggle::ToggleEvent::New_ctor();
        if(onValueChange)
            toggle->onValueChanged->AddListener(MakeDelegate(UnityAction_1<bool>*, onValueChange));
        TextMeshProUGUI* textMesh = nameText->GetComponent<TextMeshProUGUI*>();
        textMesh->SetText(il2cpp_utils::createcsstr(text));
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

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, float minValue, float maxValue, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, minValue, maxValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, false, false, 0.0f, 0.0f, anchoredPosition, onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange) {
        return CreateIncrementSetting(parent, text, decimals, increment, currentValue, true, true, minValue, maxValue, anchoredPosition, onValueChange);
    }

    IncrementSetting* CreateIncrementSetting(Transform* parent, std::string text, int decimals, float increment, float currentValue, bool hasMin, bool hasMax, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange) {
        FormattedFloatListSettingsValueController* baseSetting = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<FormattedFloatListSettingsValueController*>(), [](FormattedFloatListSettingsValueController* x){ return to_utf8(csstrtostr(x->get_name())) == "VRRenderingScale"; }), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUIIncDecSetting", il2cpp_utils::StringType::Manual);
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
        setting->OnValueChange = onValueChange;
        Transform* child = gameObject->get_transform()->GetChild(1);
        setting->Text = child->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
        setting->Text->SetText(setting->GetRoundedString());
        setting->Text->set_richText(true);
        Button* decButton = ArrayUtil::First(child->GetComponentsInChildren<Button*>());
        Button* incButton = ArrayUtil::Last(child->GetComponentsInChildren<Button*>());
        decButton->set_interactable(true);
        incButton->set_interactable(true);
        decButton->get_onClick()->AddListener(MakeDelegate(UnityAction*, (std::function<void()>)[setting](){ setting->DecButtonPressed(); }));
        incButton->get_onClick()->AddListener(MakeDelegate(UnityAction*, (std::function<void()>)[setting](){ setting->IncButtonPressed(); }));
        
        child->GetComponent<RectTransform*>()->set_sizeDelta(UnityEngine::Vector2(40, 0));
        TextMeshProUGUI* textMesh = gameObject->GetComponentInChildren<TextMeshProUGUI*>();
        textMesh->SetText(il2cpp_utils::createcsstr(text));
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

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view name, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange)
    {
        return CreateSliderSetting(parent, name, increment, value, minValue, maxValue, 1.0f, {0.0f, 0.0f}, onValueChange);
    }

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view name, float increment, float value, float minValue, float maxValue, float applyValueTime, std::function<void(float)> onValueChange)
    {
        return CreateSliderSetting(parent, name, increment, value, minValue, maxValue, applyValueTime, {0.0f, 0.0f}, onValueChange);
    }

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view name, float increment, float value, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange)
    {
        return CreateSliderSetting(parent, name, increment, value, minValue, maxValue, 1.0f, anchoredPosition, onValueChange);
    }

    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view name, float increment, float value, float minValue, float maxValue, float applyValueTime, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange)
    {
        auto valueControllerTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<FormattedFloatListSettingsValueController*>(), [](auto x) { return to_utf8(csstrtostr(x->get_name())) == "VRRenderingScale"; });

        FormattedFloatListSettingsValueController* baseSetting = Object::Instantiate(valueControllerTemplate, parent, false);
        static auto QuestUISliderSetting_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUISliderSetting");
        baseSetting->set_name(QuestUISliderSetting_cs);

        auto gameObject = baseSetting->get_gameObject();

        auto sliderSetting = gameObject->AddComponent<QuestUI::SliderSetting*>();
        static auto ValuePicker_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("ValuePicker");
        Object::Destroy(gameObject->get_transform()->Find(ValuePicker_cs)->get_gameObject());

        auto timeSliderTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TimeSlider*>(), [](auto s) { 
            if (to_utf8(csstrtostr(s->get_name())) != "RangeValuesTextSlider") return false;
            auto parent = s->get_transform()->get_parent();
            if (!parent) return false; 
            return to_utf8(csstrtostr(parent->get_name())) == "SongStart";
            });

        sliderSetting->slider = Object::Instantiate(timeSliderTemplate, gameObject->get_transform(), false);
        sliderSetting->Setup(minValue, maxValue, value, increment, applyValueTime, onValueChange);
        static auto QuestUISlider_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUISlider");
        sliderSetting->slider->set_name(QuestUISlider_cs);
        sliderSetting->slider->GetComponentInChildren<TextMeshProUGUI*>()->set_enableWordWrapping(false);
        
        auto rectTransform = reinterpret_cast<RectTransform*>(sliderSetting->slider->get_transform());
        rectTransform->set_anchorMin(Vector2(1, 0));
        rectTransform->set_anchorMax(Vector2(1, 1));
        rectTransform->set_sizeDelta(Vector2(40, 0));
        rectTransform->set_pivot(Vector2(1, 0.5f));
        rectTransform->set_anchoredPosition(Vector2(0, 0));

        Object::Destroy(baseSetting);

        static auto NameText_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("NameText"); 
        auto nameText = gameObject->get_transform()->Find(NameText_cs)->get_gameObject();
        Polyglot::LocalizedTextMeshProUGUI* localizedText = nameText->GetComponent<Polyglot::LocalizedTextMeshProUGUI*>();
        localizedText->set_enabled(false);
        localizedText->set_Key(System::String::_get_Empty());

        auto text = nameText->GetComponent<TextMeshProUGUI*>();
        text->set_text(il2cpp_utils::newcsstr(name));

        auto externalComponents = gameObject->AddComponent<ExternalComponents*>();
        externalComponents->Add(text);
        externalComponents->Add(localizedText);

        gameObject->GetComponent<LayoutElement*>()->set_preferredWidth(90);

        gameObject->SetActive(true);
        gameObject->GetComponent<RectTransform*>()->set_anchoredPosition(anchoredPosition);
        return sliderSetting;
    }

    GameObject* CreateScrollView(Transform* parent) {
        TextPageScrollView* textScrollView = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<ReleaseInfoViewController*>())->textPageScrollView, parent);
        static auto textScrollViewName = il2cpp_utils::createcsstr("QuestUIScrollView", il2cpp_utils::StringType::Manual);
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

        viewport->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        viewport->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));

        static auto parentObjName = il2cpp_utils::createcsstr("QuestUIScrollViewContent", il2cpp_utils::StringType::Manual);
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

        static auto childName = il2cpp_utils::createcsstr("QuestUIScrollViewContentContainer", il2cpp_utils::StringType::Manual);
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
        static auto name = il2cpp_utils::createcsstr("QuestUIScrollableSettingsContainer", il2cpp_utils::StringType::Manual);
        scrollTransform->get_gameObject()->set_name(name);
        return content;
    }

    InputFieldView* CreateStringSetting(Transform* parent, std::string settingsName, std::string currentValue, std::function<void(std::string)> onValueChange) {
        return CreateStringSetting(parent, settingsName, currentValue, UnityEngine::Vector2(0.0f, 0.0f), onValueChange);
    }

    InputFieldView* CreateStringSetting(Transform* parent, std::string settingsName, std::string currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(std::string)> onValueChange) {
        return CreateStringSetting(parent, settingsName, currentValue, anchoredPosition, UnityEngine::Vector3(1337.0f, 1337.0f, 1337.0f), onValueChange);
    }
    
    InputFieldView* CreateStringSetting(Transform* parent, std::string settingsName, std::string currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector3 keyboardPositionOffset, std::function<void(std::string)> onValueChange) {
        InputFieldView* originalFieldView = ArrayUtil::First(Resources::FindObjectsOfTypeAll<InputFieldView*>(), [](InputFieldView* x) { 
            return to_utf8(csstrtostr(x->get_name())) == "GuestNameInputField"; });
        GameObject* gameObj = Object::Instantiate(originalFieldView->get_gameObject(), parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUIStringSetting", il2cpp_utils::StringType::Manual);
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
        fieldView->placeholderText->GetComponent<TextMeshProUGUI*>()->SetText(il2cpp_utils::createcsstr(settingsName));
        fieldView->SetText(il2cpp_utils::createcsstr(currentValue));
        fieldView->onValueChanged = InputFieldView::InputFieldChanged::New_ctor();
        if(onValueChange) {
            fieldView->onValueChanged->AddListener(MakeDelegate(UnityAction_1<InputFieldView*>*,
                (std::function<void(InputFieldView*)>)[onValueChange](InputFieldView* fieldView){
                    onValueChange(to_utf8(csstrtostr(fieldView->get_text())));
                }));
        }
        return fieldView;
    }

    SimpleTextDropdown* CreateDropdown(Transform* parent, std::string dropdownName, std::string currentValue, std::vector<std::string> values, std::function<void(std::string)> onValueChange) {
        GameObject* gameObj = Object::Instantiate(dropdownListPrefab, parent, false);
        static auto name = il2cpp_utils::createcsstr("QuestUIDropdownList", il2cpp_utils::StringType::Manual);
        gameObj->set_name(name);
        SimpleTextDropdown* dropdown = gameObj->GetComponentInChildren<SimpleTextDropdown*>();
        dropdown->get_gameObject()->SetActive(false);

        auto* physicsRaycaster = GetPhysicsRaycasterWithCache();
        if(physicsRaycaster)
            reinterpret_cast<VRGraphicRaycaster*>(dropdown->GetComponentInChildren(csTypeOf(VRGraphicRaycaster*), true))->physicsRaycaster = physicsRaycaster;
        
        reinterpret_cast<ModalView*>(dropdown->GetComponentInChildren(csTypeOf(ModalView*), true))->container = GetDiContainer();

        static auto labelName = il2cpp_utils::createcsstr("Label", il2cpp_utils::StringType::Manual);
        GameObject* labelObject = gameObj->get_transform()->Find(labelName)->get_gameObject();
        GameObject::Destroy(labelObject->GetComponent<LocalizedTextMeshProUGUI*>());
        labelObject->GetComponent<TextMeshProUGUI*>()->SetText(il2cpp_utils::createcsstr(dropdownName));

        LayoutElement* layoutElement = gameObj->AddComponent<LayoutElement*>();
        layoutElement->set_preferredWidth(90.0f);
        layoutElement->set_preferredHeight(8.0f);

        List<Il2CppString*>* list = List<Il2CppString*>::New_ctor();
        int selectedIndex = 0;
        for(int i = 0; i < values.size(); i++){
            std::string value = values[i];
            if(currentValue == value)
                selectedIndex = i;
            list->Add(il2cpp_utils::createcsstr(value));
        }
        dropdown->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<Il2CppString*>*>(list));
        dropdown->SelectCellWithIdx(selectedIndex);

        if(onValueChange) {
            using DelegateType = System::Action_2<DropdownWithTableView*, int>*;
            dropdown->add_didSelectCellWithIdxEvent(MakeDelegate(DelegateType,
                (std::function<void(SimpleTextDropdown*, int)>)[onValueChange](SimpleTextDropdown* dropdownWithTableView, int index){
                    onValueChange(to_utf8(csstrtostr(reinterpret_cast<List<Il2CppString*>*>(dropdownWithTableView->texts)->get_Item(index))));
                }));
        }

        dropdown->get_gameObject()->SetActive(true);
        gameObj->SetActive(true);
        return dropdown;
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
            static auto backgroundGoName = il2cpp_utils::createcsstr("bg", il2cpp_utils::StringType::Manual);
            auto backgroundGo = UnityEngine::GameObject::New_ctor(backgroundGoName);
            backgroundGo->get_transform()->SetParent(gameObject->get_transform(), false);
            auto background = backgroundGo->AddComponent<Backgroundable*>();
            static auto backgroundName = il2cpp_utils::createcsstr("round-rect-panel", il2cpp_utils::StringType::Manual);
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

    GameObject* CreateColorPicker(Transform* parent, std::string text, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color, ColorChangeUIEventType)> onValueChange) {
        // use QuestUI toggle as starting point to make positioning and sizing easier
        auto fakeToggle = CreateToggle(parent, text);
        auto gameObject = fakeToggle->get_transform()->get_parent()->get_gameObject();
        Object::Destroy(fakeToggle->get_gameObject());
        
        // create element that gets toggled to contain the actual picker
        static auto name = il2cpp_utils::createcsstr("QuestUIColorPickerModal", il2cpp_utils::StringType::Manual);
        auto pickerModalGO = GameObject::New_ctor(name);
        auto pickerModalGORect = pickerModalGO->AddComponent<RectTransform*>();
        pickerModalGORect->set_anchorMin(UnityEngine::Vector2(1.0f, 1.0f));
        pickerModalGORect->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        pickerModalGORect->set_sizeDelta(UnityEngine::Vector2(40.0f, 40.0f));
        pickerModalGO->get_transform()->SetParent(parent, false);
        pickerModalGO->SetActive(false);

        // initialize the color button that toggles the picker open
        auto buttonBase = ArrayUtil::First(Resources::FindObjectsOfTypeAll<ColorPickerButtonController*>(), [](ColorPickerButtonController* x) { 
            return to_utf8(csstrtostr(x->get_name())) == "ColorPickerButtonSecondary"; });
        auto buttonGO = Object::Instantiate(buttonBase->get_gameObject(), gameObject->get_transform(), false);
        static auto buttonGOName = il2cpp_utils::createcsstr("QuestUIColorPickerButton", il2cpp_utils::StringType::Manual);
        buttonGO->set_name(buttonGOName);
        auto buttonRect = buttonGO->GetComponent<RectTransform*>();
        buttonRect->set_anchorMin(UnityEngine::Vector2(1.0f, 0.5f));
        buttonRect->set_anchorMax(UnityEngine::Vector2(1.0f, 0.5f));
        buttonRect->set_anchoredPosition(UnityEngine::Vector2(-1.0f, -0.5f));
        buttonRect->set_pivot(UnityEngine::Vector2(1.0f, 0.5f));

        auto colorPickerButtonController = buttonGO->GetComponent<ColorPickerButtonController*>();
        colorPickerButtonController->SetColor(defaultColor);

        // initialize the picker itself
        auto pickerBase = ArrayUtil::First(Resources::FindObjectsOfTypeAll<HSVPanelController*>(), [](HSVPanelController* x) { 
            return to_utf8(csstrtostr(x->get_name())) == "HSVColorPicker"; });
        auto pickerGO = Object::Instantiate(pickerBase->get_gameObject(), pickerModalGORect, false);
        static auto pickerGOName = il2cpp_utils::createcsstr("QuestUIColorPickerController", il2cpp_utils::StringType::Manual);
        pickerGO->set_name(pickerGOName);
        auto hsvPanelController = pickerGO->GetComponent<HSVPanelController*>();
        static auto searchName = il2cpp_utils::createcsstr("ColorPickerButtonPrimary", il2cpp_utils::StringType::Manual);
        Object::Destroy(pickerGO->get_transform()->Find(searchName)->get_gameObject());
        hsvPanelController->set_color(defaultColor);
        auto pickerRect = pickerGO->GetComponent<UnityEngine::RectTransform*>();
        pickerRect->set_pivot(UnityEngine::Vector2(0.25f, 0.77f));
        pickerRect->set_localScale(UnityEngine::Vector3(0.75f, 0.75f, 0.75f));

        // event bindings
        using DelegateType = System::Action_2<UnityEngine::Color, ColorChangeUIEventType>*;
        hsvPanelController->add_colorDidChangeEvent(MakeDelegate(DelegateType,
            ((std::function<void(UnityEngine::Color, ColorChangeUIEventType)>)[colorPickerButtonController, onValueChange](UnityEngine::Color color, ColorChangeUIEventType eventType) {
                colorPickerButtonController->SetColor(color);
                if(onValueChange)
                    onValueChange(color, eventType);
            })
        ));
        colorPickerButtonController->button->get_onClick()->AddListener(MakeDelegate(UnityAction*, 
            (std::function<void()>)[pickerModalGO]() {
                pickerModalGO->SetActive(!pickerModalGO->get_activeSelf());
            }
        ));
        ExternalComponents* externalComponents = buttonGO->AddComponent<ExternalComponents*>();
        externalComponents->Add(pickerModalGORect);
        return buttonGO;
    }

    QuestUI::ModalColorPicker* CreateColorPickerModal(UnityEngine::Transform* parent, std::string name, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color)> onDone, std::function<void()> onCancel, std::function<void(UnityEngine::Color)> onChange)
    {
        // base.CreateObject
        auto modal = CreateModal(parent, Vector2(135, 75), Vector2(0, 0), nullptr, false);
        auto gameObject = modal->get_gameObject();

        auto windowTransform = reinterpret_cast<RectTransform*>(gameObject->get_transform());
        
        static auto QuestUIModalColorPicker_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIModalColorPicker");
        windowTransform->set_name(QuestUIModalColorPicker_cs);

        auto colorPicker = gameObject->AddComponent<ModalColorPicker*>();
        colorPicker->Setup(defaultColor, onChange, onDone, onCancel);
        colorPicker->modalView = modal;

        auto rgbTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<RGBPanelController*>(), [](auto x){ return to_utf8(csstrtostr(x->get_name())) == "RGBColorPicker"; });
        auto hsvTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<HSVPanelController*>(), [](auto x){ return to_utf8(csstrtostr(x->get_name())) == "HSVColorPicker"; });
        auto currentColorTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<ImageView*>(), [](auto x){ 
            if (to_utf8(csstrtostr(x->get_gameObject()->get_name())) != "SaberColorA") return false;
            auto parent = x->get_transform()->get_parent();
            if (!parent) return false;
            return to_utf8(csstrtostr(parent->get_name())) == "ColorSchemeView";
        });

        auto rgbController = Object::Instantiate(rgbTemplate, gameObject->get_transform(), false);
        static auto QuestUIRGBPanel_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIRGBPanel");
        rgbController->set_name(QuestUIRGBPanel_cs);
        auto rectTransform = reinterpret_cast<RectTransform*>(rgbController->get_transform());
        rectTransform->set_anchoredPosition(Vector2(0, 3));
        rectTransform->set_anchorMin(Vector2(0, 0.25f));
        rectTransform->set_anchorMax(Vector2(0, 0.25f));
        colorPicker->rgbPanel = rgbController;
        
        std::function<void(UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType)> OnChange = std::bind(&ModalColorPicker::OnChange, colorPicker, std::placeholders::_1, std::placeholders::_2);
        auto delegate = il2cpp_utils::MakeDelegate<System::Action_2<UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType>*>(classof(System::Action_2<UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType>*), OnChange);
        rgbController->add_colorDidChangeEvent(delegate);

        auto hsvController = Object::Instantiate(hsvTemplate, gameObject->get_transform(), false);
        static auto QuestUIHSVPanel_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIHSVPanel");
        hsvController->set_name(QuestUIHSVPanel_cs);
        rectTransform = reinterpret_cast<RectTransform*>(hsvController->get_transform());
        rectTransform->set_anchoredPosition(Vector2(0, 3));
        rectTransform->set_anchorMin(Vector2(0.75f, 0.5f));
        rectTransform->set_anchorMax(Vector2(0.75f, 0.5f));
        colorPicker->hsvPanel = hsvController;
        
        hsvController->add_colorDidChangeEvent(delegate);

        auto colorImage = Object::Instantiate(currentColorTemplate, gameObject->get_transform(), false);
        static auto QuestUICurrentColor_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUICurrentColor");
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
        static auto name = il2cpp_utils::createcsstr("QuestUIModalPrefab", il2cpp_utils::StringType::Manual);

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
        static Il2CppString* BGname = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BG");
        int childCount = modal->get_transform()->get_childCount();
        for (int i = 0; i < childCount; i++) {
            auto* child = modal->get_transform()->GetChild(i)->GetComponent<RectTransform*>();

            if (child->get_gameObject()->get_name()->Equals(BGname)) {
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

        static auto name = il2cpp_utils::createcsstr("QuestUICreateScrollableModalContainer", il2cpp_utils::StringType::Manual);

        scrollTransform->get_gameObject()->set_name(name);
        return content;
    }
    
    CustomCellListTableData* CreateCustomCellList(Transform* parent, Vector2 anchoredPosition, Vector2 sizeDelta, CustomCellListWrapper* listWrapper)
    {
        auto list = CreateCustomCellList(parent, listWrapper);
        auto rect = list->GetComponent<RectTransform*>();

        rect->set_sizeDelta(sizeDelta);
        rect->set_anchoredPosition(anchoredPosition);
        
        auto layout = list->GetComponent<LayoutElement*>();
        layout->set_flexibleHeight(sizeDelta.y);
        layout->set_minHeight(sizeDelta.y);
        layout->set_preferredHeight(sizeDelta.y);
        layout->set_flexibleHeight(sizeDelta.x);
        layout->set_minHeight(sizeDelta.x);
        layout->set_preferredHeight(sizeDelta.x);

        return list;
    }

    CustomCellListTableData* CreateCustomCellList(Transform* parent, Vector2 sizeDelta, CustomCellListWrapper* listWrapper)
    {
        auto list = CreateCustomCellList(parent, listWrapper);
        auto rect = list->GetComponent<RectTransform*>();
        rect->set_sizeDelta(sizeDelta);
        auto layout = list->GetComponent<LayoutElement*>();
        layout->set_flexibleHeight(sizeDelta.y);
        layout->set_minHeight(sizeDelta.y);
        layout->set_preferredHeight(sizeDelta.y);
        layout->set_flexibleHeight(sizeDelta.x);
        layout->set_minHeight(sizeDelta.x);
        layout->set_preferredHeight(sizeDelta.x);

        return list;
    }

    // BSML CustomListTag
    CustomCellListTableData* CreateCustomCellList(Transform* parent, CustomCellListWrapper* listWrapper)
    {
        static auto QuestUICustomListContainer_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUICustomListContainer");
        auto container = GameObject::New_ctor(QuestUICustomListContainer_cs)->AddComponent<RectTransform*>();;
        auto layoutElement = container->get_gameObject()->AddComponent<LayoutElement*>();
        container->SetParent(parent, false);

        static auto QuestUICustomList_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUICustomList");
        auto gameObject = GameObject::New_ctor(QuestUICustomList_cs);
        gameObject->get_transform()->SetParent(container, false);
        gameObject->SetActive(false);

        static auto DropdownTableView_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("DropdownTableView");
        auto canvasTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Canvas*>(), [&](auto x) { return x->get_name()->Equals(DropdownTableView_cs); });

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
        auto tableData = container->get_gameObject()->AddComponent<CustomCellListTableData*>();
        tableData->listWrapper = listWrapper;
        tableData->tableView = tableView;
        
        tableView->preallocatedCells = Array<TableView::CellsGroup*>::NewLength(0);
        tableView->isInitialized = false;
        tableView->scrollView = scrollView;

        static auto Viewport_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Viewport");
        auto viewport = GameObject::New_ctor(Viewport_cs)->AddComponent<RectTransform*>();
        viewport->SetParent(gameObject->GetComponent<RectTransform*>(), false);
        viewport->get_gameObject()->AddComponent<RectMask2D*>();
        gameObject->GetComponent<ScrollRect*>()->set_viewport(viewport);

        static auto Content_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Content");
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
        tableView->get_gameObject()->SetActive(true);
        return tableData;
    }
    
    // not used anymore, but might be useful to keep around?
    /*
    Button* CreatePageButton(std::string name, Transform* parent, std::string buttonTemplate, Vector2 anchoredPosition, Vector2 sizeDelta, std::function<void(void)> onClick, Sprite* icon = nullptr)
    {
        Il2CppString* templCS = il2cpp_utils::newcsstr(buttonTemplate.data());
        auto orig = ArrayUtil::Last(Resources::FindObjectsOfTypeAll<Button*>(), [&](auto x) {
            return x->get_name()->Equals(templCS);
        });

        if (!icon)
            icon = orig->GetComponentInChildren<Image*>()->get_sprite();
        
        auto btn = Object::Instantiate(orig, parent, false);
        btn->GetComponentInChildren<Image*>()->set_sprite(icon);
        btn->set_name(il2cpp_utils::newcsstr(name.data()));
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
    
    CustomCellListTableData* CreateScrollableCustomCellList(UnityEngine::Transform* parent, CustomCellListWrapper* listWrapper)
    {
        return CreateScrollableCustomCellList(parent, {60.0f, 35.0f}, listWrapper);
    }

    CustomCellListTableData* CreateScrollableCustomCellList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, CustomCellListWrapper* listWrapper)
    {
        return CreateScrollableCustomCellList(parent, {0.0f, 0.0f}, sizeDelta, listWrapper);
    }
    
    CustomCellListTableData* CreateScrollableCustomCellList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, CustomCellListWrapper* listWrapper)
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

        auto list = CreateCustomCellList(vertical->get_transform(), Vector2 (sizeDelta.x, sizeDelta.y - 16.0f), listWrapper);
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
        static auto QuestUIListContainer_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIListContainer");
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

        static auto QuestUIList_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIList");
        auto gameObject = GameObject::New_ctor(QuestUIList_cs);
        gameObject->get_transform()->SetParent(container, false);
        gameObject->SetActive(false);

        static auto DropdownTableView_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("DropdownTableView");
        auto canvasTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Canvas*>(), [&](auto x) { return x->get_name()->Equals(DropdownTableView_cs); });

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

        static auto Viewport_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Viewport");
        auto viewport = GameObject::New_ctor(Viewport_cs)->AddComponent<RectTransform*>();
        viewport->SetParent(gameObject->GetComponent<RectTransform*>(), false);
        viewport->get_gameObject()->AddComponent<RectMask2D*>();
        gameObject->GetComponent<ScrollRect*>()->set_viewport(viewport);

        static auto Content_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Content");
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
            auto delegate = il2cpp_utils::MakeDelegate<DelegateType*>(classof(DelegateType*), fun);
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
        static auto QuestUIListContainer_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIListContainer");
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

        static auto QuestUIList_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIList");
        auto gameObject = GameObject::New_ctor(QuestUIList_cs);
        gameObject->get_transform()->SetParent(container, false);
        gameObject->SetActive(false);

        static auto DropdownTableView_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("DropdownTableView");
        auto canvasTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Canvas*>(), [&](auto x) { return x->get_name()->Equals(DropdownTableView_cs); });

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

        static auto Viewport_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Viewport");
        auto viewport = GameObject::New_ctor(Viewport_cs)->AddComponent<RectTransform*>();
        viewport->SetParent(gameObject->GetComponent<RectTransform*>(), false);
        viewport->get_gameObject()->AddComponent<RectMask2D*>();
        gameObject->GetComponent<ScrollRect*>()->set_viewport(viewport);

        static auto Content_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Content");
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
            auto delegate = il2cpp_utils::MakeDelegate<DelegateType*>(classof(DelegateType*), fun);
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

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::initializer_list<std::u16string> values, std::function<void(int)> onCellWithIdxClicked)
    {
        auto segmentedControlTemplate = ArrayUtil::First(Resources::FindObjectsOfTypeAll<HMUI::TextSegmentedControl*>(), [](auto x){ 
            if (to_utf8(csstrtostr(x->get_name())) != "TextSegmentedControl") return false;
            auto parent = x->get_transform()->get_parent();
            if (!parent) return false;
            std::string parentName = to_utf8(csstrtostr(parent->get_name()));
            return parentName == "GameplaySetupViewController" || parentName == "BaseGameplaySetupWrapper";
        });

        auto segmentedControlObj = Object::Instantiate(segmentedControlTemplate->get_gameObject(), parent, false);
        segmentedControlObj->SetActive(false);
        static auto QuestUITextSegmentedControl_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUITextSegmentedControl");
        segmentedControlObj->set_name(QuestUITextSegmentedControl_cs);
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
            auto delegate = il2cpp_utils::MakeDelegate<System::Action_2<HMUI::SegmentedControl*, int>*>(classof(System::Action_2<HMUI::SegmentedControl*, int>*), fun);
            control->add_didSelectCellEvent(delegate);
        }

        int childCount = result->get_transform()->get_childCount();
        for (int i = 0; i < childCount; i++) 
        {
            getLogger().info("child %d/%d", i, childCount);
            Object::DestroyImmediate(result->get_transform()->GetChild(0)->get_gameObject());
        }
        
        result->set_texts(values);

        segmentedControlObj->SetActive(true);
        return result;
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::initializer_list<std::u16string> values, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, sizeDelta, values, onCellWithIdxClicked);
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::initializer_list<std::u16string> values, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, {80, 10}, values, onCellWithIdxClicked);
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, {80, 10}, {}, onCellWithIdxClicked);
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::vector<std::u16string> values, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, sizeDelta, values, onCellWithIdxClicked);
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::vector<std::u16string> values, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, {0, 0}, {80, 10}, values, onCellWithIdxClicked);
    }

    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::vector<std::u16string> values, std::function<void(int)> onCellWithIdxClicked)
    {
        return CreateTextSegmentedControl(parent, anchoredPosition, sizeDelta, values, onCellWithIdxClicked);
    }
}
