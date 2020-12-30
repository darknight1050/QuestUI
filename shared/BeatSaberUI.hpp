#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "ArrayUtil.hpp"

#include "CustomTypes/Components/Settings/IncrementSetting.hpp"

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
#include "HMUI/InputFieldView.hpp"
#include "HMUI/ModalView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/TMP_FontAsset.hpp"
#include "VRUIControls/PhysicsRaycasterWithCache.hpp"

namespace QuestUI::BeatSaberUI {

    GlobalNamespace::MainFlowCoordinator* getMainFlowCoordinator();

    TMPro::TMP_FontAsset* getMainTextFont();

    UnityEngine::Sprite* getEditIcon();

    VRUIControls::PhysicsRaycasterWithCache* GetPhysicsRaycasterWithCache();

    void ClearCache();

    HMUI::ViewController* CreateViewController(System::Type* type);

    template<class T = HMUI::ViewController*>
    T CreateViewController() {
        return (T)CreateViewController(typeof(T));
    }

    HMUI::FlowCoordinator* CreateFlowCoordinator(System::Type* type);

    template<class T = HMUI::FlowCoordinator*>
    T CreateFlowCoordinator() {
        return (T)CreateFlowCoordinator(typeof(T));
    }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition);

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);
    
    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string text, bool italic = true);

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string text, bool italic, UnityEngine::Vector2 anchoredPosition);

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);

    void SetButtonText(UnityEngine::UI::Button* button, std::string text);

    void SetButtonTextSize(UnityEngine::UI::Button* button, float fontSize);

    void ToggleButtonWordWrapping(UnityEngine::UI::Button* button, bool enableWordWrapping);

    void SetButtonIcon(UnityEngine::UI::Button* button, UnityEngine::Sprite* icon);

    void SetButtonBackground(UnityEngine::UI::Button* button, UnityEngine::Sprite* background);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonText, std::string buttonTemplate, UnityEngine::Events::UnityAction* onClick = nullptr);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonText, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction* onClick = nullptr);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonText, std::string buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, UnityEngine::Events::UnityAction* onClick = nullptr);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonText, UnityEngine::Events::UnityAction* onClick = nullptr);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction* onClick = nullptr);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, UnityEngine::Events::UnityAction* onClick = nullptr);

    UnityEngine::UI::Image* CreateImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);

    UnityEngine::Sprite* Base64ToSprite(std::string& base64, int width, int height);   

    UnityEngine::Sprite* FileToSprite(std::string& filePath, int width, int height);

    UnityEngine::UI::GridLayoutGroup* CreateGridLayoutGroup(UnityEngine::Transform* parent);

    UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent);

    UnityEngine::UI::VerticalLayoutGroup* CreateVerticalLayoutGroup(UnityEngine::Transform* parent);
    
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string text, UnityEngine::Events::UnityAction_1<bool>* onToggle = nullptr);

    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string text, bool currentValue, UnityEngine::Events::UnityAction_1<bool>* onToggle = nullptr);
    
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction_1<bool>* onToggle = nullptr);
    
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string text, bool currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction_1<bool>* onToggle = nullptr);
    
    //UnityEngine::GameObject* CreateLoadingIndicator(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition);

    HMUI::HoverHint* AddHoverHint(UnityEngine::GameObject* gameObject, std::string text);

    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string text, int decimals, float increment, float currentValue, UnityEngine::Events::UnityAction_1<float>* onValueChange = nullptr);
    
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string text, int decimals, float increment, float currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction_1<float>* onValueChange = nullptr);

    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string text, int decimals, float increment, float currentValue, float minValue, float maxValue, UnityEngine::Events::UnityAction_1<float>* onValueChange = nullptr);
    
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string text, int decimals, float increment, float currentValue, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction_1<float>* onValueChange = nullptr);
    
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string text, int decimals, float increment, float currentValue, bool hasMin, bool hasMax, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction_1<float>* onValueChange = nullptr);

    UnityEngine::GameObject* CreateScrollView(UnityEngine::Transform* parent);

    UnityEngine::GameObject* CreateScrollableSettingsContainer(UnityEngine::Transform* parent);

    //HMUI::ModalView* CreateModalView(UnityEngine::Transform* parent);

    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::string settingsName, std::string currentValue, UnityEngine::Events::UnityAction_1<Il2CppString*>* onValueChange = nullptr);

    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::string settingsName, std::string currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction_1<Il2CppString*>* onValueChange = nullptr);

}
