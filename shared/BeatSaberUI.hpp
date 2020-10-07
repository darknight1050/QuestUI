#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "ArrayUtil.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/GameplayModifierToggle.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/GridLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/HoverHint.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "CustomTypes/Components/QuestUIScrollView.hpp"
#include "TMPro/TMP_FontAsset.hpp"

namespace QuestUI::BeatSaberUI {

    GlobalNamespace::MainFlowCoordinator* getMainFlowCoordinator();

    TMPro::TMP_FontAsset* getMainTextFont();

    HMUI::ViewController* CreateViewController(System::Type* type);

    template<class T = HMUI::ViewController*>
    T CreateViewController() {
        static auto name = il2cpp_utils::createcsstr("BSMLViewController", il2cpp_utils::StringType::Permanent);
        T viewController = UnityEngine::GameObject::New_ctor(name)->AddComponent<T>();
        UnityEngine::Object::DontDestroyOnLoad(viewController->get_gameObject());

        UnityEngine::RectTransform* rectTransform = viewController->get_rectTransform();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        return viewController;
    }

    HMUI::FlowCoordinator* CreateFlowCoordinator(System::Type* type);

    template<class T = HMUI::FlowCoordinator*>
    T CreateFlowCoordinator() {
        static auto name = il2cpp_utils::createcsstr("BSMLFlowCoordinator", il2cpp_utils::StringType::Permanent);
        T flowCoordinator = UnityEngine::GameObject::New_ctor(name)->AddComponent<T>();
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

    UnityEngine::UI::GridLayoutGroup* CreateGridLayoutGroup(UnityEngine::Transform* parent);

    UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent);

    UnityEngine::UI::VerticalLayoutGroup* CreateVerticalLayoutGroup(UnityEngine::Transform* parent);
    
    UnityEngine::GameObject* CreateToggle(UnityEngine::Transform* parent, std::string text, UnityEngine::Events::UnityAction_1<bool>* onToggle);

    UnityEngine::GameObject* CreateToggle(UnityEngine::Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction_1<bool>* onToggle);
    
    UnityEngine::GameObject* CreateLoadingIndicator(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition);

    HMUI::HoverHint* AddHoverHint(UnityEngine::GameObject* gameObject, std::string text);

    UnityEngine::GameObject* CreateIncrementSetting(UnityEngine::Transform* parent, std::string text, int decimals, float increment, float currentValue);
    
    UnityEngine::GameObject* CreateIncrementSetting(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, std::string text, int decimals, float increment, float currentValue);

    UnityEngine::GameObject* CreateScrollableSettingsContainer(UnityEngine::Transform* parent);

    UnityEngine::GameObject* CreateScrollView(UnityEngine::Transform* parent);

    UnityEngine::GameObject* CreateModalView(UnityEngine::Transform* parent);
    
    UnityEngine::GameObject* CreateKeyboard(UnityEngine::Transform* parent);
}
