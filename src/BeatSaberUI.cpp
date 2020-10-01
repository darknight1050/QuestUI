#include "BeatSaberUI.hpp"

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "System/Convert.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace TMPro;

namespace BeatSaberUI {
    
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

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition) {
        return CreateText(parent, text, anchoredPosition, UnityEngine::Vector2(60.0f, 10.0f));
    }

    TextMeshProUGUI* CreateText(Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        GameObject* gameObj = GameObject::New_ctor(il2cpp_utils::createcsstr("CustomUIText"));
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
    
        button->set_name(il2cpp_utils::createcsstr("CustomUIButton"));

        RectTransform* rectTransform = (RectTransform*)button->get_transform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));

        SetButtonText(button, buttonText);
        if(icon)
            SetButtonIcon(button, icon);
        return button;
    }

    Image* CreateImage(Transform* parent, Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta) {
        Image* image = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("CustomImage"))->AddComponent<Image*>();
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

}
