#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "ArrayUtil.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/TMP_FontAsset.hpp"

namespace BeatSaberUI {

    GlobalNamespace::MainFlowCoordinator* getMainFlowCoordinator();

    TMPro::TMP_FontAsset* getMainTextFont();

    template<class T = HMUI::ViewController*>
    T CreateViewController() {
        T viewController = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("BSMLViewController"))->AddComponent<T>();
        UnityEngine::Object::DontDestroyOnLoad(viewController->get_gameObject());

        UnityEngine::RectTransform* rectTransform = viewController->get_rectTransform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        return viewController;
    }

    template<class T = HMUI::FlowCoordinator*>
    T CreateFlowCoordinator() {
        T flowCoordinator = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("BSMLFlowCoordinator"))->AddComponent<T>();
        flowCoordinator->baseInputModule = getMainFlowCoordinator()->baseInputModule;
        return flowCoordinator;
    }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition);

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);

    void SetButtonText(UnityEngine::UI::Button* button, std::string text);

    void SetButtonTextSize(UnityEngine::UI::Button* button, float fontSize);

    void ToggleButtonWordWrapping(UnityEngine::UI::Button* button, bool enableWordWrapping);

    void SetButtonIcon(UnityEngine::UI::Button* button, UnityEngine::Sprite* icon);

    void SetButtonBackground(UnityEngine::UI::Button* button, UnityEngine::Sprite* background);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, UnityEngine::Events::UnityAction* onClick, std::string buttonText, UnityEngine::Sprite* icon);
    
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction* onClick, std::string buttonText, UnityEngine::Sprite* icon);
    
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonTemplate, UnityEngine::Events::UnityAction* onClick, std::string buttonText, UnityEngine::Sprite* icon);
    
    UnityEngine::UI::Image* CreateImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);

    UnityEngine::Sprite* Base64ToSprite(std::string base64, int width, int height);
}