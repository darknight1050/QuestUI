#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "ArrayUtil.hpp"

#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "CustomTypes/Components/Settings/SliderSetting.hpp"
#include "CustomTypes/Components/List/CustomCellListWrapper.hpp"
#include "CustomTypes/Components/List/CustomListTableData.hpp"
#include "CustomTypes/Components/SegmentedControl/CustomTextSegmentedControlData.hpp"
#include "CustomTypes/Components/ModalColorPicker.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/GameplayModifierToggle.hpp"
#include "GlobalNamespace/ColorChangeUIEventType.hpp"

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
#include "HMUI/ImageView.hpp"
#include "HMUI/SimpleTextDropdown.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/TMP_FontAsset.hpp"

#include "VRUIControls/PhysicsRaycasterWithCache.hpp"

namespace QuestUI::BeatSaberUI {

    GlobalNamespace::MainFlowCoordinator* GetMainFlowCoordinator();

    TMPro::TMP_FontAsset* GetMainTextFont();

    UnityEngine::Material* GetMainUIFontMaterial();

    UnityEngine::Sprite* GetEditIcon();

    VRUIControls::PhysicsRaycasterWithCache* GetPhysicsRaycasterWithCache();

    void ClearCache();

    UnityEngine::GameObject* CreateCanvas();

    HMUI::ViewController* CreateViewController(System::Type* type);

    template<class T = HMUI::ViewController*>
    T CreateViewController() {
        return (T)CreateViewController(reinterpret_cast<System::Type*>(csTypeOf(T)));
    }

    HMUI::FlowCoordinator* CreateFlowCoordinator(System::Type* type);

    template<class T = HMUI::FlowCoordinator*>
    T CreateFlowCoordinator() {
        return (T)CreateFlowCoordinator(reinterpret_cast<System::Type*>(csTypeOf(T)));
    }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::u16string_view text, UnityEngine::Vector2 anchoredPosition);
    inline TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string_view text, UnityEngine::Vector2 anchoredPosition)
    { return CreateText(parent, to_utf16(text), anchoredPosition); }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::u16string_view text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);
    inline TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string_view text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta)
    { return CreateText(parent, to_utf16(text), anchoredPosition, sizeDelta); }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::u16string_view text, bool italic = true);
    inline TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string_view text, bool italic = true)
    { return CreateText(parent, to_utf16(text), italic); }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::u16string_view text, bool italic, UnityEngine::Vector2 anchoredPosition);
    inline TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string_view text, bool italic, UnityEngine::Vector2 anchoredPosition)
    { return CreateText(parent, to_utf16(text), italic, anchoredPosition); }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::u16string_view text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);
    inline TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string_view text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta)
    { return CreateText(parent, to_utf16(text), italic, anchoredPosition, sizeDelta); }

    void SetButtonText(UnityEngine::UI::Button* button, std::u16string_view text);
    inline void SetButtonText(UnityEngine::UI::Button* button, std::string_view text)
    { SetButtonText(button, to_utf16(text)); }

    void SetButtonTextSize(UnityEngine::UI::Button* button, float fontSize);

    void ToggleButtonWordWrapping(UnityEngine::UI::Button* button, bool enableWordWrapping);

    void SetButtonIcon(UnityEngine::UI::Button* button, UnityEngine::Sprite* icon);

    void SetButtonBackground(UnityEngine::UI::Button* button, UnityEngine::Sprite* background);

    /// @brief sets the sprites for a given button
    /// @param button the button to swap sprites for
    /// @param inactive, for when not hovering over the button
    /// @param active for when hovering over the button
    void SetButtonSprites(UnityEngine::UI::Button* button, UnityEngine::Sprite* inactive, UnityEngine::Sprite* active);

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::u16string_view buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr);
    inline UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string_view buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr)
    { return CreateUIButton(parent, to_utf16(buttonText), buttonTemplate, anchoredPosition, onClick); }

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::u16string_view buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr);
    inline UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string_view buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr)
    { return CreateUIButton(parent, to_utf16(buttonText), buttonTemplate, anchoredPosition, sizeDelta, onClick); }

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::u16string_view buttonText, std::function<void()> onClick = nullptr);
    inline UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string_view buttonText, std::function<void()> onClick = nullptr)
    { return CreateUIButton(parent, to_utf16(buttonText), onClick); }

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::u16string_view buttonText, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr);
    inline UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string_view buttonText, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr)
    { return CreateUIButton(parent, to_utf16(buttonText), anchoredPosition, onClick); }

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::u16string_view buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr);
    inline UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string_view buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr)
    { return CreateUIButton(parent, to_utf16(buttonText), anchoredPosition, sizeDelta, onClick); }

    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::u16string_view buttonText, std::string_view buttonTemplate, std::function<void()> onClick = nullptr);
    inline UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, std::string_view buttonText, std::string_view buttonTemplate, std::function<void()> onClick = nullptr)
    { return CreateUIButton(parent, to_utf16(buttonText), buttonTemplate, onClick); }

    HMUI::ImageView* CreateImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);

    UnityEngine::Sprite* Base64ToSprite(std::string_view base64);   
    inline UnityEngine::Sprite* Base64ToSprite(std::string_view base64, int width, int height)
    { return Base64ToSprite(base64); }


    UnityEngine::Sprite* FileToSprite(std::string_view filePath);
    inline UnityEngine::Sprite* FileToSprite(std::string_view filePath, int width, int height)
    { return FileToSprite(filePath); }

    UnityEngine::Sprite* VectorToSprite(std::vector<uint8_t> bytes);

    UnityEngine::Sprite* ArrayToSprite(Array<uint8_t>* bytes);

    UnityEngine::UI::GridLayoutGroup* CreateGridLayoutGroup(UnityEngine::Transform* parent);

    UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent);

    UnityEngine::UI::VerticalLayoutGroup* CreateVerticalLayoutGroup(UnityEngine::Transform* parent);

    // https://github.com/monkeymanboy/BeatSaberMarkupLanguage/blob/master/BeatSaberMarkupLanguage/Tags/ModifierContainerTag.cs
    UnityEngine::UI::VerticalLayoutGroup* CreateModifierContainer(UnityEngine::Transform* parent);
    
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::u16string_view text, std::function<void(bool)> onToggle = nullptr);
    inline UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string_view text, std::function<void(bool)> onToggle = nullptr)
    { return CreateToggle(parent, to_utf16(text), onToggle); }

    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::u16string_view text, bool currentValue, std::function<void(bool)> onToggle = nullptr);
    inline UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string_view text, bool currentValue, std::function<void(bool)> onToggle = nullptr)
    { return CreateToggle(parent, to_utf16(text), currentValue, onToggle); }
    
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::u16string_view text, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onToggle = nullptr);
    inline UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string_view text, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onToggle = nullptr)
    { return CreateToggle(parent, to_utf16(text), anchoredPosition, onToggle); }
    
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::u16string_view text, bool currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onToggle = nullptr);
    inline UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, std::string_view text, bool currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onToggle = nullptr)
    { return CreateToggle(parent, to_utf16(text), currentValue, anchoredPosition, onToggle); }

    //UnityEngine::GameObject* CreateLoadingIndicator(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition);

    HMUI::HoverHint* AddHoverHint(UnityEngine::GameObject* gameObject, std::u16string_view text);
    inline HMUI::HoverHint* AddHoverHint(UnityEngine::GameObject* gameObject, std::string_view text)
    { return AddHoverHint(gameObject, to_utf16(text)); }

    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::u16string_view text, int decimals, float increment, float currentValue, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string_view text, int decimals, float increment, float currentValue, std::function<void(float)> onValueChange = nullptr)
    { return CreateIncrementSetting(parent, to_utf16(text), decimals, increment, currentValue, onValueChange); }

    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::u16string_view text, int decimals, float increment, float currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string_view text, int decimals, float increment, float currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr)
    { return CreateIncrementSetting(parent, to_utf16(text), decimals, increment, currentValue, anchoredPosition, onValueChange); }

    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::u16string_view text, int decimals, float increment, float currentValue, float minValue, float maxValue, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string_view text, int decimals, float increment, float currentValue, float minValue, float maxValue, std::function<void(float)> onValueChange = nullptr)
    { return CreateIncrementSetting(parent, to_utf16(text), decimals, increment, currentValue, minValue, maxValue, onValueChange); }

    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::u16string_view text, int decimals, float increment, float currentValue, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string_view text, int decimals, float increment, float currentValue, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr)
    { return CreateIncrementSetting(parent, to_utf16(text), decimals, increment, currentValue, minValue, maxValue, anchoredPosition, onValueChange); }

    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::u16string_view text, int decimals, float increment, float currentValue, bool hasMin, bool hasMax, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, std::string_view text, int decimals, float increment, float currentValue, bool hasMin, bool hasMax, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr)
    { return CreateIncrementSetting(parent, to_utf16(text), decimals, increment, currentValue, hasMin, hasMax, minValue, maxValue, anchoredPosition, onValueChange); }

    /// @brief Creates a slider setting
    /// @param parent what to parent it to
    /// @param text the text displayed in front to let the user know what they are editing
    /// @param increment the increment each "tick" should do
    /// @param value the initial value it should be
    /// @param minValue the minimum value for the slider
    /// @param maxValue the maximum value for the slider
    /// @param onValueChange callback called after the user has not interacted with the slider for 1 second
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::u16string_view text, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view text, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange = nullptr)
    { return CreateSliderSetting(parent, to_utf16(text), increment, value, minValue, maxValue, onValueChange); }
    
    /// @brief Creates a slider setting
    /// @param parent what to parent it to
    /// @param text the text displayed in front to let the user know what they are editing
    /// @param increment the increment each "tick" should do
    /// @param value the initial value it should be
    /// @param minValue the minimum value for the slider
    /// @param maxValue the maximum value for the slider
    /// @param applyValueTime the amount of seconds to wait after the user stopped interacting to call the callback
    /// @param onValueChange callback called after the user has not interacted with the slider for 1 second
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::u16string_view text, float increment, float value, float minValue, float maxValue, float applyValueTime, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view text, float increment, float value, float minValue, float maxValue, float applyValueTime, std::function<void(float)> onValueChange = nullptr)
    { return CreateSliderSetting(parent, to_utf16(text), increment, value, minValue, maxValue, applyValueTime, onValueChange); }

    /// @brief Creates a slider setting
    /// @param parent what to parent it to
    /// @param text the text displayed in front to let the user know what they are editing
    /// @param increment the increment each "tick" should do
    /// @param value the initial value it should be
    /// @param minValue the minimum value for the slider
    /// @param maxValue the maximum value for the slider
    /// @param anchoredPosition the position
    /// @param onValueChange callback called after the user has not interacted with the slider for 1 second
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::u16string_view text, float increment, float value, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view text, float increment, float value, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr)
    { return CreateSliderSetting(parent, to_utf16(text), increment, value, minValue, maxValue, anchoredPosition, onValueChange); }
    
    /// @brief Creates a slider setting
    /// @param parent what to parent it to
    /// @param text the text displayed in front to let the user know what they are editing
    /// @param increment the increment each "tick" should do
    /// @param value the initial value it should be
    /// @param minValue the minimum value for the slider
    /// @param maxValue the maximum value for the slider
    /// @param applyValueTime the amount of seconds to wait after the user stopped interacting to call the callback
    /// @param anchoredPosition the position
    /// @param onValueChange callback called after the user has not interacted with the slider for 1 second
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::u16string_view text, float increment, float value, float minValue, float maxValue, float applyValueTime, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    inline QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, std::string_view text, float increment, float value, float minValue, float maxValue, float applyValueTime, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr)
    { return CreateSliderSetting(parent, to_utf16(text), increment, value, minValue, maxValue, applyValueTime, anchoredPosition, onValueChange); }
    
    UnityEngine::GameObject* CreateScrollView(UnityEngine::Transform* parent);

    UnityEngine::GameObject* CreateScrollableSettingsContainer(UnityEngine::Transform* parent);

    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::u16string_view settingsName, std::string_view currentValue, std::function<void(std::string_view)> onValueChange = nullptr);
    inline HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::string_view settingsName, std::string_view currentValue, std::function<void(std::string_view)> onValueChange = nullptr)
    { return CreateStringSetting(parent, to_utf16(settingsName), currentValue, onValueChange); }

    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::u16string_view settingsName, std::string_view currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(std::string_view)> onValueChange = nullptr);
    inline HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::string_view settingsName, std::string_view currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(std::string_view)> onValueChange = nullptr)
    { return CreateStringSetting(parent, to_utf16(settingsName), currentValue, anchoredPosition, onValueChange); }

    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::u16string_view settingsName, std::string_view currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector3 keyboardPositionOffset, std::function<void(std::string_view)> onValueChange = nullptr);
    inline HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, std::string_view settingsName, std::string_view currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector3 keyboardPositionOffset, std::function<void(std::string_view)> onValueChange = nullptr)
    { return CreateStringSetting(parent, to_utf16(settingsName), currentValue, anchoredPosition, keyboardPositionOffset, onValueChange); }

    HMUI::SimpleTextDropdown* CreateDropdown(UnityEngine::Transform* parent, std::u16string_view dropdownName, std::u16string_view currentValue, std::vector<std::u16string> values, std::function<void(std::u16string_view)> onValueChange = nullptr);
    inline HMUI::SimpleTextDropdown* CreateDropdown(UnityEngine::Transform* parent, std::string_view dropdownName, std::u16string_view currentValue, std::vector<std::u16string> values, std::function<void(std::u16string_view)> onValueChange = nullptr)
    { return CreateDropdown(parent, to_utf16(dropdownName), currentValue, values, onValueChange); }

    HMUI::SimpleTextDropdown* CreateDropdown(UnityEngine::Transform* parent, std::u16string_view dropdownName, std::string_view currentValue, std::vector<std::string> values, std::function<void(std::string_view)> onValueChange = nullptr);
    inline HMUI::SimpleTextDropdown* CreateDropdown(UnityEngine::Transform* parent, std::string_view dropdownName, std::string_view currentValue, std::vector<std::string> values, std::function<void(std::string_view)> onValueChange = nullptr)
    { return CreateDropdown(parent, to_utf16(dropdownName), currentValue, values, onValueChange); }

    //For handleSide, a different int corresponds to each side. 0-left, 1-right, 2-bottom, 3-top, 4-full
    UnityEngine::GameObject* CreateFloatingScreen(UnityEngine::Vector2 screenSize, UnityEngine::Vector3 position, UnityEngine::Vector3 rotation, float curvatureRadius = 0, bool hasBackground = true, bool createHandle = true, int handleSide = 4);
    
    UnityEngine::GameObject* CreateColorPicker(UnityEngine::Transform* parent, std::u16string_view text, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType)> onValueChange = nullptr);
    inline UnityEngine::GameObject* CreateColorPicker(UnityEngine::Transform* parent, std::string_view text, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType)> onValueChange = nullptr)
    { return CreateColorPicker(parent, to_utf16(text), defaultColor, onValueChange); }
    
    /// @brief Creates a color picker modal
    /// @param parent what to parent it to
    /// @param name the text to display
    /// @param defaultColor the color to start off with
    /// @param onDone the callback to call when color picking is done
    /// @param onCancel the callback to call when the user cancels
    /// @param onChange the callback to call when the color changes at all
    QuestUI::ModalColorPicker* CreateColorPickerModal(UnityEngine::Transform* parent, std::u16string_view name, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color)> onDone = nullptr, std::function<void()> onCancel = nullptr, std::function<void(UnityEngine::Color)> onChange = nullptr);
    inline QuestUI::ModalColorPicker* CreateColorPickerModal(UnityEngine::Transform* parent, std::string_view name, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color)> onDone = nullptr, std::function<void()> onCancel = nullptr, std::function<void(UnityEngine::Color)> onChange = nullptr)
    { return CreateColorPickerModal(parent, to_utf16(name), defaultColor, onDone, onCancel, onChange); }
    
    /// @brief creates a modal that can be used to display information
    /// @param parent what to parent it to
    /// @param onBlockerClicked callback that gets called when clicking next to the modal, leaving it empty makes it just dismiss the modal
    /// @param sizeDelta size of the object
    /// @param anchoredPosition position of the modal
    /// @param dismissOnBlockerClicked whether to auto dismiss when the blocker (outside) is clicked
    /// @return created modal
    HMUI::ModalView* CreateModal(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, UnityEngine::Vector2 anchoredPosition, std::function<void(HMUI::ModalView*)> onBlockerClicked, bool dismissOnBlockerClicked = true);

    /// @brief creates a modal that can be used to display information
    /// @param parent what to parent it to
    /// @param onBlockerClicked callback that gets called when clicking next to the modal, leaving it empty makes it just dismiss the modal
    /// @param sizeDelta size of the object
    /// @param dismissOnBlockerClicked whether to auto dismiss when the blocker (outside) is clicked
    /// @return created modal
    HMUI::ModalView* CreateModal(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(HMUI::ModalView*)> onBlockerClicked, bool dismissOnBlockerClicked = true);

    /// @brief creates a modal that can be used to display information
    /// @param parent what to parent it to
    /// @param onBlockerClicked callback that gets called when clicking next to the modal, leaving it empty makes it just dismiss the modal
    /// @param dismissOnBlockerClicked whether to auto dismiss when the blocker (outside) is clicked
    /// @return created modal
    HMUI::ModalView* CreateModal(UnityEngine::Transform* parent, std::function<void(HMUI::ModalView*)> onBlockerClicked, bool dismissOnBlockerClicked = true);

    /// @brief creates a modal that can be used to display information
    /// @param parent what to parent it to
    /// @param dismissOnBlockerClicked whether to auto dismiss when the blocker (outside) is clicked
    /// @return created modal
    HMUI::ModalView* CreateModal(UnityEngine::Transform* parent, bool dismissOnBlockerClicked = true);

    /// @brief creates a scrollable container for the given modal, with exact fit
    /// @param modal the modal to create a container for
    /// @return GameObject container
    UnityEngine::GameObject* CreateScrollableModalContainer(HMUI::ModalView* modal);

    /// @brief creates a custom celled list
    /// @param parent what to parent it to
    /// @param listWrapper a wrapper to determine the way the cells will look and where the data comes from, allocate with c++ new!
    /// @return CustomCellListTableData* useful if you need to talk to the specific instance of the datasource
    CustomCellListTableData* CreateCustomCellList(UnityEngine::Transform* parent, CustomCellListWrapper* listWrapper);
    
    /// @brief creates a custom celled list
    /// @param parent what to parent it to
    /// @param sizeDelta the size of the list
    /// @param listWrapper a wrapper to determine the way the cells will look and where the data comes from, allocate with c++ new!
    /// @return CustomCellListTableData* useful if you need to talk to the specific instance of the datasource
    CustomCellListTableData* CreateCustomCellList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, CustomCellListWrapper* listWrapper);

    /// @brief creates a custom celled list
    /// @param parent what to parent it to
    /// @param listWrapper a wrapper to determine the way the cells will look and where the data comes from, allocate with c++ new!
    /// @param anchoredPosition the position of the list
    /// @param sizeDelta the size of the list
    /// @param listWrapper a wrapper to determine the way the cells will look and where the data comes from, allocate with c++ new!
    /// @return CustomCellListTableData* useful if you need to talk to the specific instance of the datasource
    CustomCellListTableData* CreateCustomCellList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, CustomCellListWrapper* listWrapper);

    /// @brief creates a custom celled list
    /// @param parent what to parent it to
    /// @param listWrapper a wrapper to determine the way the cells will look and where the data comes from, allocate with c++ new!
    /// @return CustomCellListTableData* useful if you need to talk to the specific instance of the datasource
    CustomCellListTableData* CreateScrollableCustomCellList(UnityEngine::Transform* parent, CustomCellListWrapper* listWrapper);

    /// @brief creates a custom celled list
    /// @param parent what to parent it to
    /// @param sizeDelta the size of the list
    /// @param listWrapper a wrapper to determine the way the cells will look and where the data comes from, allocate with c++ new!
    /// @return CustomCellListTableData* useful if you need to talk to the specific instance of the datasource
    CustomCellListTableData* CreateScrollableCustomCellList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, CustomCellListWrapper* listWrapper);\

    /// @brief creates a custom celled list
    /// @param parent what to parent it to
    /// @param anchoredPosition the position of the list
    /// @param sizeDelta the size of the list
    /// @param listWrapper a wrapper to determine the way the cells will look and where the data comes from, allocate with c++ new!
    /// @return CustomCellListTableData* useful if you need to talk to the specific instance of the datasource
    CustomCellListTableData* CreateScrollableCustomCellList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, CustomCellListWrapper* listWrapper);

    // using statement in BeatSaberUI namespace so that people can use it as QuestUI::BeatSaberUI::CustomCellInfo;
    using CustomCellInfo = CustomListTableData::CustomCellInfo;

    /// @brief creates a custom list based on the level/pack/simple lists
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    /// @return CustomListTableData* so you can add your data to the list
    CustomListTableData* CreateList(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked = nullptr);

    /// @brief creates a custom list based on the level/pack/simple lists
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    /// @return CustomListTableData* so you can add your data to the list
    CustomListTableData* CreateList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr);

    /// @brief creates a custom list based on the level/pack/simple lists
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    /// @return CustomListTableData* so you can add your data to the list
    CustomListTableData* CreateList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr);

    /// @brief creates a custom list based on the level/pack/simple lists
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    /// @return CustomListTableData* so you can add your data to the list
    CustomListTableData* CreateScrollableList(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked = nullptr);

    /// @brief creates a custom list based on the level/pack/simple lists
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    /// @return CustomListTableData* so you can add your data to the list
    CustomListTableData* CreateScrollableList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr);
    
    /// @brief creates a custom list based on the level/pack/simple lists
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    /// @return CustomListTableData* so you can add your data to the list
    CustomListTableData* CreateScrollableList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr);
    
    /// @brief creates a List with a custom HMUI::TableView::IDataSource
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    HMUI::TableView::IDataSource* CreateCustomSourceList(System::Type* type, UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr);

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T>
    T CreateCustomSourceList(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, {0.0f, 0.0f}, {35.0f, 60.0f}, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T>
    T CreateCustomSourceList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, {0.0f, 0.0f}, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T>
    T CreateCustomSourceList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, anchoredPosition, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a List with a custom IDataSource
    /// @param type type of class that implements HMUI::TableView::IDataSource
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    HMUI::TableView::IDataSource* CreateScrollableCustomSourceList(System::Type* type, UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr);

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T>
    T CreateScrollableCustomSourceList(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateScrollableCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, {0.0f, 0.0f}, {35.0f, 60.0f}, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T>
    T CreateScrollableCustomSourceList(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateScrollableCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, {0.0f, 0.0f}, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T>
    T CreateScrollableCustomSourceList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateScrollableCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, anchoredPosition, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::initializer_list<std::u16string> values, std::function<void(int)> onCellWithIdxClicked = nullptr);
    
    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::initializer_list<std::u16string> values, std::function<void(int)> onCellWithIdxClicked = nullptr);
    
    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::initializer_list<std::u16string> values, std::function<void(int)> onCellWithIdxClicked = nullptr);
    
    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked = nullptr);

    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::vector<std::u16string> values, std::function<void(int)> onCellWithIdxClicked);
    
    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::vector<std::u16string> values, std::function<void(int)> onCellWithIdxClicked);

    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked);
    
    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::vector<std::u16string> values, std::function<void(int)> onCellWithIdxClicked);
}
