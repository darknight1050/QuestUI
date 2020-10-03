#include "BeatSaberUI.hpp"

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "System/Convert.hpp"
#include <string>

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace TMPro;
using namespace HMUI;

namespace QuestUI::BeatSaberUI {
    
    GlobalNamespace::MainFlowCoordinator* getMainFlowCoordinator() {
        static GlobalNamespace::MainFlowCoordinator* mainFlowCoordinator = nullptr;
        if(!mainFlowCoordinator)
            mainFlowCoordinator = Object::FindObjectOfType<GlobalNamespace::MainFlowCoordinator*>();
        return mainFlowCoordinator;
    }

    TMP_FontAsset* getMainTextFont() {
        static TMP_FontAsset* mainTextFont = nullptr;
        if(!mainTextFont){
            mainTextFont = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TMP_FontAsset*>(), [](TMP_FontAsset* x) { return to_utf8(csstrtostr(x->get_name())) == "Teko-Medium SDF No Glow"; });
        }
        return mainTextFont;
    }
    
    HMUI::ViewController* CreateViewController(System::Type* type) {
        HMUI::ViewController* viewController = (HMUI::ViewController*)UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("QuestUIViewController"))->AddComponent(type);
        UnityEngine::Object::DontDestroyOnLoad(viewController->get_gameObject());

        UnityEngine::RectTransform* rectTransform = viewController->get_rectTransform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        return viewController;
    }
    
    HMUI::FlowCoordinator* CreateFlowCoordinator(System::Type* type) {
        HMUI::FlowCoordinator* flowCoordinator = (HMUI::FlowCoordinator*)UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("QuestUIFlowCoordinator"))->AddComponent(type);
        flowCoordinator->baseInputModule = getMainFlowCoordinator()->baseInputModule;
        return flowCoordinator;
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, anchoredPosition, UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        GameObject* gameObj = GameObject::New_ctor(il2cpp_utils::createcsstr("QuestUIText"));
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
        Polyglot::LocalizedTextMeshProUGUI* localizer = button->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>();
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

    Button* CreateUIButton(Transform* parent, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, Events::UnityAction* onClick, std::string buttonText, Sprite* icon){
        Button* button = CreateUIButton(parent, buttonTemplate, anchoredPosition, onClick, buttonText, icon);
        ((RectTransform*)button->get_transform())->set_sizeDelta(sizeDelta);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, Events::UnityAction* onClick, std::string buttonText, Sprite* icon){
        Button* button = CreateUIButton(parent, buttonTemplate, onClick, buttonText, icon);
        ((RectTransform*)button->get_transform())->set_anchoredPosition(anchoredPosition);
        return button;
    }

    Button* CreateUIButton(Transform* parent, std::string buttonTemplate, Events::UnityAction* onClick, std::string buttonText, Sprite* icon){
        Button* button = Object::Instantiate(ArrayUtil::Last(Resources::FindObjectsOfTypeAll<Button*>(), [&buttonTemplate](Button* x) { return to_utf8(csstrtostr(x->get_name())) == buttonTemplate; }), parent, false);
        button->set_onClick(Button::ButtonClickedEvent::New_ctor());
        if(onClick)
            button->get_onClick()->AddListener(onClick);
    
        button->set_name(il2cpp_utils::createcsstr("QuestUIButton"));

        RectTransform* rectTransform = (RectTransform*)button->get_transform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));

        SetButtonText(button, buttonText);
        if(icon)
            SetButtonIcon(button, icon);
        return button;
    }

    Image* CreateImage(Transform* parent, Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        Image* image = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("QuestUIImage"))->AddComponent<Image*>();
        image->get_transform()->SetParent(parent, false);
        image->set_sprite(sprite);
        RectTransform* rectTransform = (RectTransform*)image->get_transform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchoredPosition(anchoredPosition);
        rectTransform->set_sizeDelta(sizeDelta);
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
    
    UnityEngine::UI::GridLayoutGroup* CreateGridLayoutGroup(UnityEngine::Transform* parent) {
        UnityEngine::RectTransform* rectTransform = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("QuestUIGridLayoutGroup"), typeof(UnityEngine::UI::GridLayoutGroup*), typeof(UnityEngine::UI::ContentSizeFitter*))->GetComponent<UnityEngine::RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        return rectTransform->GetComponent<UnityEngine::UI::GridLayoutGroup*>();
    }
    
    UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent) {
        UnityEngine::GameObject* gameObject = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("QuestUIHorizontalLayoutGroup"), typeof(UnityEngine::UI::HorizontalLayoutGroup*));
        
        UnityEngine::UI::ContentSizeFitter* contentSizeFitter = gameObject->AddComponent<UnityEngine::UI::ContentSizeFitter*>();
        contentSizeFitter->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);

        UnityEngine::RectTransform* rectTransform = gameObject->GetComponent<UnityEngine::RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        
        gameObject->AddComponent<UnityEngine::UI::LayoutElement*>();
        return rectTransform->GetComponent<UnityEngine::UI::HorizontalLayoutGroup*>();
    }
    
    UnityEngine::UI::VerticalLayoutGroup* CreateVerticalLayoutGroup(UnityEngine::Transform* parent) {
        UnityEngine::GameObject* gameObject = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("QuestUIVerticalLayoutGroup"), typeof(UnityEngine::UI::VerticalLayoutGroup*));
        
        UnityEngine::UI::ContentSizeFitter* contentSizeFitter = gameObject->AddComponent<UnityEngine::UI::ContentSizeFitter*>();
        contentSizeFitter->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);

        UnityEngine::RectTransform* rectTransform = gameObject->GetComponent<UnityEngine::RectTransform*>();
        rectTransform->SetParent(parent, false);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        
        gameObject->AddComponent<UnityEngine::UI::LayoutElement*>();
        return rectTransform->GetComponent<UnityEngine::UI::VerticalLayoutGroup*>();
    }

    GameObject* CreateToggle(Transform* Parent, std::string Text, UnityEngine::Vector2 AnchoredPosition, UnityEngine::UI::Toggle::ToggleEvent* onToggle, bool hoverHint, std::string hoverHintText)
    {
        GameplayModifierToggle* baseSetting = Object::Instantiate(ArrayUtil::First(Resources::FindObjectsOfTypeAll<GameplayModifierToggle*>(), [](GameplayModifierToggle* x){ return to_utf8(csstrtostr(x->get_name())) == "InstaFail"; }), Parent, false);
        baseSetting->set_name(il2cpp_utils::createcsstr("BSMLCheckboxSetting"));

        GameObject* gameObject = baseSetting->get_gameObject();
        gameObject->SetActive(false);

        Object::Destroy(baseSetting);
        Object::Destroy(gameObject->get_transform()->GetChild(0)->get_gameObject());
        HoverHint* hvrHint = gameObject->GetComponent<HoverHint*>();
        if(!hoverHint && hoverHintText == "")
        {
            Object::Destroy(hvrHint); 
        }
        else
        {
            hvrHint->set_text(il2cpp_utils::createcsstr(hoverHintText, il2cpp_utils::StringType::Permanent));
        }
        
        Toggle* tgle = gameObject->GetComponent<Toggle*>();
        tgle->onValueChanged = onToggle;
        TextMeshProUGUI* text = gameObject->GetComponentInChildren<TextMeshProUGUI*>();
        text->set_fontSize(5); //Change some settings to conform more to the List Dropdown/IncDec settings controllers
        text->SetText(il2cpp_utils::createcsstr(Text));
        RectTransform* RT = text->get_rectTransform();
        RT->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, 0.0f));
        RT->set_anchoredPosition(AnchoredPosition);
        RT->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));;

        LayoutElement* layout = gameObject->GetComponent<LayoutElement*>(); //If Beat Games decides to add one later down the road.
        if (layout == nullptr) layout = gameObject->AddComponent<LayoutElement*>(); //For the time being, they dont have one, so time to add one myself!
        layout->set_preferredWidth(90); //Again, to conform to List Dropdown/IncDec settings controllers
        layout->set_preferredHeight(8);

        gameObject->SetActive(true);
        return gameObject;
    }

    GameObject* CreateToggle(Transform* Parent, std::string Text, UnityEngine::UI::Toggle::ToggleEvent* onToggle, bool hoverHint, std::string hoverHintText)
    {
        return CreateToggle(Parent, Text, UnityEngine::Vector2(0.0f, 0.0f), onToggle);
    }
}
