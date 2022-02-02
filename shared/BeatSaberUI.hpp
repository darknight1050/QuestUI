#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "ArrayUtil.hpp"

#include "CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "CustomTypes/Components/Settings/SliderSetting.hpp"
#include "CustomTypes/Components/List/CustomListTableData.hpp"
#include "CustomTypes/Components/SegmentedControl/CustomTextSegmentedControlData.hpp"
#include "CustomTypes/Components/ModalColorPicker.hpp"
#include "CustomTypes/Components/ClickableText.hpp"
#include "CustomTypes/Components/ClickableImage.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/GameplayModifierToggle.hpp"
#include "GlobalNamespace/ColorChangeUIEventType.hpp"
#include "GlobalNamespace/IVRPlatformHelper.hpp"

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

#include <concepts>
#include <type_traits>

#include <base64_rfc4648.hpp>

namespace QuestUI::BeatSaberUI {

    /// @brief gets the main flow coordinator
    GlobalNamespace::MainFlowCoordinator* GetMainFlowCoordinator();

    /// @brief gets main ui text font, used internally mostly
    TMPro::TMP_FontAsset* GetMainTextFont();

    /// @brief gets main ui font material, used internally mostly
    UnityEngine::Material* GetMainUIFontMaterial();

    /// @brief gets edit icon, used internally mostly
    UnityEngine::Sprite* GetEditIcon();

    /// @brief gets the PhysicsRaycasterWithCache used internally mostly
    VRUIControls::PhysicsRaycasterWithCache* GetPhysicsRaycasterWithCache();

    /// @brief gets the IVR platform helper used internally mostly
    GlobalNamespace::IVRPlatformHelper* GetIVRPlatformHelper();

    /// @brief clears cached objects
    void ClearCache();  

    /// @brief creates a canvas to attach things to
    /// @return gameobject which you can parent things to
    UnityEngine::GameObject* CreateCanvas();

    /// @brief Creates a viewcontroller
    /// @param type type of the viewcontroller
    /// @return Created viewcontroller as HMUI::ViewController*
    HMUI::ViewController* CreateViewController(System::Type* type);

    /// @brief Creates a viewcontroller of type T
    /// @tparam T viewcontroller Type
    /// @return Created viewcontroller as T
    template<class T = HMUI::ViewController*>
    requires(std::is_convertible_v<T, HMUI::ViewController*>)
    T CreateViewController() {
        return reinterpret_cast<T>(CreateViewController(reinterpret_cast<System::Type*>(csTypeOf(T))));
    }

    /// @brief Creates a flowcoordinator
    /// @param type type of the flowcoordinator
    /// @return Created flowcoordinator as HMUI::FlowCoordinator*
    HMUI::FlowCoordinator* CreateFlowCoordinator(System::Type* type);

    /// @brief Creates a flowcoordinator of type T
    /// @tparam T flowcoordinator Type
    /// @return Created flowcoordinator as T
    template<class T = HMUI::FlowCoordinator*>
    requires(std::is_convertible_v<T, HMUI::FlowCoordinator*>)
    T CreateFlowCoordinator() {
        return (T)CreateFlowCoordinator(reinterpret_cast<System::Type*>(csTypeOf(T)));
    }

    template<typename T, typename U>
    concept QuestUIConvertible = std::is_convertible_v<T, U>;

    template<typename T>
    concept HasTransform = requires (T a) { {a->get_transform() } -> QuestUIConvertible<UnityEngine::Transform*>; };

    template<typename T>
    concept HasGameObject = requires (T a) { {a->get_gameObject() } -> QuestUIConvertible<UnityEngine::GameObject*>; };

    /// @brief Creates a text object
    /// @param parent what to parent it to
    /// @param text the string to display
    /// @param italic should the text be italic
    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, StringW text, bool italic = true);

    /// @brief Creates a text object
    /// @param parent what to parent it to
    /// @param text the string to display
    /// @param anchoredPosition the location of the text
    /// @param sizeDelta the size of the text
    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);

    /// @brief Creates a text object
    /// @param parent what to parent it to
    /// @param text the string to display
    /// @param anchoredPosition the location of the text
    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition);

    /// @brief Creates a text object
    /// @param parent what to parent it to
    /// @param text the string to display
    /// @param italic should the text be italic
    /// @param anchoredPosition the location of the text
    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition);

    /// @brief Creates a text object
    /// @param parent what to parent it to
    /// @param text the string to display
    /// @param italicc should the text be italic
    /// @param anchoredPosition the location of the text
    /// @param sizeDelta the size of the text
    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);

    /// @brief Overload for creating a text object that allows you to pass in anything that has a ->get_transform() method
    /// @param parent what to parent it to
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline TMPro::TextMeshProUGUI* CreateText(T parent, TArgs...args) {
        return CreateText(parent->get_transform(), args...);
    }

    /// @brief Creates clickable text, almost like a URL
    /// @param parent what to parent it to
    /// @param text the text to display
    /// @param anchoredPosition position of the text
    /// @param onClick what to run when clicked
    /// @return created clickable text
    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr);

    /// @brief Creates clickable text, almost like a URL
    /// @param parent what to parent it to
    /// @param text the text to display
    /// @param anchoredPosition position of the text
    /// @param sizeDelta size of the object
    /// @param onClick what to run when clicked
    /// @return created clickable text
    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr);

    /// @brief Creates clickable text, almost like a URL
    /// @param parent what to parent it to
    /// @param text the text to display
    /// @param italic is the text italic?
    /// @param onClick what to run when clicked
    /// @return created clickable text
    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, bool italic = true, std::function<void()> onClick = nullptr);

    /// @brief Creates clickable text, almost like a URL
    /// @param parent what to parent it to
    /// @param text the text to display
    /// @param italic is the text italic?
    /// @param anchoredPosition position of the text
    /// @param onClick what to run when clicked
    /// @return created clickable text
    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr);

    /// @brief Creates clickable text, almost like a URL
    /// @param parent what to parent it to
    /// @param text the text to display
    /// @param italic is the text italic?
    /// @param anchoredPosition position of the text
    /// @param sizeDelta size of the object
    /// @param onClick what to run when clicked
    /// @return created clickable text
    ClickableText* CreateClickableText(UnityEngine::Transform* parent, StringW text, bool italic, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr);

    /// @brief Overload for creating a Clickable Text object that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline ClickableText* CreateClickableText(T parent, TArgs...args) {
        return CreateClickableText(parent->get_transform(), args...);
    }

    /// @brief set the button text on the passed in button
    /// @param button the button to change the text on
    /// @param text the text to set
    void SetButtonText(UnityEngine::UI::Button* button, StringW text);

    /// @brief set the button text size on the passed in button
    /// @param button the button to change the text size on
    /// @param fontSize the font size to change the text to
    void SetButtonTextSize(UnityEngine::UI::Button* button, float fontSize);

    /// @brief set the button wordwrapping on the passed in button
    /// @param button the button to change the text size on
    /// @param enableWordWrapping whether to allow wordwrapping
    void ToggleButtonWordWrapping(UnityEngine::UI::Button* button, bool enableWordWrapping);

    /// @brief set the icon on the passed in button
    /// @param button the button to change icon of
    /// @param icon the icon to set
    void SetButtonIcon(UnityEngine::UI::Button* button, UnityEngine::Sprite* icon);

    /// @brief set the background on the passed in button
    /// @param button the button to change the background on
    /// @param icon the background to set
    void SetButtonBackground(UnityEngine::UI::Button* button, UnityEngine::Sprite* background);

    /// @brief sets the sprites for a given button
    /// @param button the button to swap sprites for
    /// @param inactive, for when not hovering over the button
    /// @param active for when hovering over the button
    void SetButtonSprites(UnityEngine::UI::Button* button, UnityEngine::Sprite* inactive, UnityEngine::Sprite* active);

    /// @brief Creates a button to click and have an action happen
    /// @param parent what to parent it to
    /// @param buttonText the text the butotn displays
    /// @param onClick callback to run when the button is clicked
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, StringW buttonText, std::function<void()> onClick = nullptr);

    /// @brief Creates a button to click and have an action happen
    /// @param parent what to parent it to
    /// @param buttonText the text the butotn displays
    /// @param buttonTemplate the name of a button to use as a base
    /// @param onClick callback to run when the button is clicked
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, StringW buttonText, std::string_view buttonTemplate, std::function<void()> onClick = nullptr);

    /// @brief Creates a button to click and have an action happen
    /// @param parent what to parent it to
    /// @param buttonText the text the butotn displays
    /// @param buttonTemplate the name of a button to use as a base
    /// @param anchoredPosition position of the button
    /// @param onClick callback to run when the button is clicked
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, StringW buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr);

    /// @brief Creates a button to click and have an action happen
    /// @param parent what to parent it to
    /// @param buttonText the text the butotn displays
    /// @param buttonTemplate the name of a button to use as a base
    /// @param anchoredPosition position of the button
    /// @param sizeDelta size of the button
    /// @param onClick callback to run when the button is clicked
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, StringW buttonText, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr);

    /// @brief Creates a button to click and have an action happen
    /// @param parent what to parent it to
    /// @param buttonText the text the butotn displays
    /// @param anchoredPosition position of the button
    /// @param onClick callback to run when the button is clicked
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, StringW buttonText, UnityEngine::Vector2 anchoredPosition, std::function<void()> onClick = nullptr);

    /// @brief Creates a button to click and have an action happen
    /// @param parent what to parent it to
    /// @param buttonText the text the butotn displays
    /// @param anchoredPosition position of the button
    /// @param sizeDelta size of the button
    /// @param onClick callback to run when the button is clicked
    UnityEngine::UI::Button* CreateUIButton(UnityEngine::Transform* parent, StringW buttonText, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr);
    
    /// @brief Overload for creating a UI Button that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline ClickableText* CreateUIButton(T parent, TArgs...args) {
        return CreateUIButton(parent->get_transform(), args...);
    }

    /// @brief Creates clickable text, almost like a URL
    /// @param parent what to parent it to
    /// @param sprite the image to use
    /// @param anchoredPosition position of the image
    /// @param sizeDelta size of the object
    /// @param onClick what to run when clicked
    /// @return created clickable image
    QuestUI::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick = nullptr);
    
    /// @brief Creates clickable text, almost like a URL
    /// @param parent what to parent it to
    /// @param sprite the image to use
    /// @param onClick what to run when clicked
    /// @return created clickable image
    QuestUI::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, std::function<void()> onClick = nullptr);
    
    /// @brief Overload for creating a Clickable image that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline ClickableText* CreateClickableImage(T parent, TArgs...args) {
        return CreateClickableImage(parent->get_transform(), args...);
    }

    /// @brief Creates an image
    /// @param parent what to parent it to
    /// @param sprite the sprite to display
    /// @param anchoredPosition the position of the image
    /// @param sizeDelta the size of the image
    HMUI::ImageView* CreateImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition = {}, UnityEngine::Vector2 sizeDelta = {});

    /// @brief Overload for creating an Image that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline HMUI::ImageView* CreateImage(T parent, TArgs...args) {
        return CreateImage(parent->get_transform(), args...);
    }

    /// @brief loads an image from a file path
    /// @param filePath the path to load the image from
    UnityEngine::Sprite* FileToSprite(std::string_view filePath);

    /// @brief loads an image from a file path [[deprecated]]
    /// @param filePath the path to load the image from
    /// @param width the width of the image
    /// @param height the height of the image
    [[deprecated]] inline UnityEngine::Sprite* FileToSprite(std::string_view filePath, int width, int height)
    { return FileToSprite(filePath); }

    /// @brief loads an image from the provided vector byte data
    /// @param bytes the bytedata
    /// @return loaded image
    UnityEngine::Sprite* VectorToSprite(std::vector<uint8_t> bytes);

    /// @brief loads an image from the provided ArrayW byte data
    /// @param bytes the bytedata
    /// @return loaded image
    UnityEngine::Sprite* ArrayToSprite(ArrayW<uint8_t> bytes);

    /// @brief decodes a base64 string to turn it into an image
    /// @param base64Str the base64 encoded image data
    /// @return loaded image
    static UnityEngine::Sprite* Base64ToSprite(std::string_view base64Str) {

        using base64     = cppcodec::base64_rfc4648;

        ArrayW<uint8_t> bytes(base64::decoded_max_size(base64Str.size()));

        base64::decode(bytes.begin(), bytes.size(), base64Str.data(), base64Str.size());

        return ArrayToSprite(bytes);
    }

    /// @brief decodes a base64 string to turn it into an image [[deprecated]]
    /// @param base64Str the base64 encoded image data
    /// @return loaded image
    [[deprecated]] inline UnityEngine::Sprite* Base64ToSprite(std::string_view base64, int width, int height)
    { return Base64ToSprite(base64); }

    /// @brief Creates a grid layout group for you
    /// @param parent what to parent it to
    /// @return the grid layout object
    UnityEngine::UI::GridLayoutGroup* CreateGridLayoutGroup(UnityEngine::Transform* parent);

    /// @brief Overload for creating a Grid layout that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    template<HasTransform T>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::UI::GridLayoutGroup* CreateGridLayoutGroup(T parent) {
        return CreateGridLayoutGroup(parent->get_transform());
    }

    /// @brief Creates a horizontal layout group for you
    /// @param parent what to parent it to
    /// @return the horizontal layout object
    UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent);

    /// @brief Overload for creating a Horizontal layout group that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the horizontal layout object
    template<HasTransform T>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(T parent) {
        return CreateHorizontalLayoutGroup(parent->get_transform());
    }

    /// @brief Creates a vertical layout group for you
    /// @param parent what to parent it to
    /// @return the vertical layout object
    UnityEngine::UI::VerticalLayoutGroup* CreateVerticalLayoutGroup(UnityEngine::Transform* parent);

    /// @brief Overload for creating a Vertical layout group that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the vertical layout object
    template<HasTransform T>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::UI::VerticalLayoutGroup* CreateVerticalLayoutGroup(T parent) {
        return CreateVerticalLayoutGroup(parent->get_transform());
    }

    // https://github.com/monkeymanboy/BeatSaberMarkupLanguage/blob/master/BeatSaberMarkupLanguage/Tags/ModifierContainerTag.cs
    /// @brief Creates a container used to put modifier buttons in
    /// @param parent what to parent it to
    /// @return vertical layout group to parent a modifier button to
    UnityEngine::UI::VerticalLayoutGroup* CreateModifierContainer(UnityEngine::Transform* parent);
    
    /// @brief Overload for creating a Modifier Container that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return vertical layout group to parent a modifier button to
    template<HasTransform T>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::UI::VerticalLayoutGroup* CreateModifierContainer(T parent) {
        return CreateModifierContainer(parent->get_transform());
    }

    /// @brief Creates a toggle that looks like the modifier buttons seen in the gameplay setup menu
    /// @param parent what to parent it to
    /// @param buttonText the text to display on the button
    /// @param currentValue is the toggle true or false at this moment
    /// @param iconSprite the sprite for the icon
    /// @param onClick what to run when the button is clicked
    /// @param anchoredPosition the position of the button
    UnityEngine::UI::Toggle* CreateModifierButton(UnityEngine::Transform* parent, StringW buttonText, bool currentValue, UnityEngine::Sprite* iconSprite, std::function<void(bool)> const& onClick = nullptr, UnityEngine::Vector2 anchoredPosition = {0,0});

    /// @brief Creates a toggle that looks like the modifier buttons seen in the gameplay setup menu
    /// @param parent what to parent it to
    /// @param buttonText the text to display on the button
    /// @param currentValue is the toggle true or false at this moment
    /// @param onClick what to run when the button is clicked
    /// @param anchoredPosition the position of the button
    inline UnityEngine::UI::Toggle* CreateModifierButton(UnityEngine::Transform* parent, StringW buttonText, bool currentValue, std::function<void(bool)> const& onClick = nullptr, UnityEngine::Vector2 anchoredPosition = {0,0}) {
        return CreateModifierButton(parent, buttonText, currentValue, nullptr, onClick, anchoredPosition);
    }

    /// @brief Overload for creating a modifier button that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the created toggle
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::UI::Toggle* CreateModifierButton(T parent, TArgs...args) {
        return CreateModifierButton(parent->get_transform(), args...);
    }

    /// @brief creates a toggle to turn things off / on
    /// @param parent what to parent it to
    /// @param text the label to give to the toggle
    /// @param onToggle what to do when the toggle is clicked
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, StringW text, std::function<void(bool)> onToggle = nullptr);

    /// @brief creates a toggle to turn things off / on
    /// @param parent what to parent it to
    /// @param text the label to give to the toggle
    /// @param currentValue the current value of the toggle
    /// @param onToggle what to do when the toggle is clicked
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, StringW text, bool currentValue, std::function<void(bool)> onToggle = nullptr);

    /// @brief creates a toggle to turn things off / on
    /// @param parent what to parent it to
    /// @param text the label to give to the toggle
    /// @param anchoredPosition the position of the toggle
    /// @param onToggle what to do when the toggle is clicked
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, StringW text, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onToggle = nullptr);

    /// @brief creates a toggle to turn things off / on
    /// @param parent what to parent it to
    /// @param text the label to give to the toggle
    /// @param currentValue the current value of the toggle
    /// @param anchoredPosition the position of the toggle
    /// @param onToggle what to do when the toggle is clicked
    UnityEngine::UI::Toggle* CreateToggle(UnityEngine::Transform* parent, StringW text, bool currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(bool)> onToggle = nullptr);

    /// @brief Overload for creating a toggle that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the created toggle
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::UI::Toggle* CreateToggle(T parent, TArgs...args) {
        return CreateToggle(parent->get_transform(), args...);
    }

    //UnityEngine::GameObject* CreateLoadingIndicator(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition);

    /// @brief Adds a hover hint to an object so that when it is hovered a message displays
    /// @param gameObject the object to add it to
    /// @param text the text to display
    /// @return the created hoverhint
    HMUI::HoverHint* AddHoverHint(UnityEngine::GameObject* gameObject, StringW text);
    
    /// @brief Overload for creating a toggle that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param gameObject the object to add it to
    /// @return the created hoverhint
    template<HasGameObject T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::GameObject*>)
    inline HMUI::HoverHint* AddHoverHint(T obj, TArgs...args) {
        return AddHoverHint(obj->get_gameObject(), args...);
    }

    /// @brief Creates an incerement setting, meaning a float value with arrows to go up and down
    /// @param parent what to parent it to
    /// @param text label to give to the setting
    /// @param decimals amount of decimals to show
    /// @param increment what to increment/decrement the value by
    /// @param currentValue the starting value
    /// @param onValueChange what to run when the value is changed
    /// @return the created increment setting
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, StringW text, int decimals, float increment, float currentValue, std::function<void(float)> onValueChange = nullptr);
    
    /// @brief Creates an incerement setting, meaning a float value with arrows to go up and down
    /// @param parent what to parent it to
    /// @param text label to give to the setting
    /// @param decimals amount of decimals to show
    /// @param increment what to increment/decrement the value by
    /// @param currentValue the starting value
    /// @param anchoredPosition the position of the setting
    /// @param onValueChange what to run when the value is changed
    /// @return the created increment setting
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, StringW text, int decimals, float increment, float currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    
    /// @brief Creates an incerement setting, meaning a float value with arrows to go up and down
    /// @param parent what to parent it to
    /// @param text label to give to the setting
    /// @param decimals amount of decimals to show
    /// @param increment what to increment/decrement the value by
    /// @param minValue the minimum value the setting is allowed to be
    /// @param maxValue the maximum value the setting is allowed to be
    /// @param currentValue the starting value
    /// @param onValueChange what to run when the value is changed
    /// @return the created increment setting
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, StringW text, int decimals, float increment, float currentValue, float minValue, float maxValue, std::function<void(float)> onValueChange = nullptr);
    
    /// @brief Creates an incerement setting, meaning a float value with arrows to go up and down
    /// @param parent what to parent it to
    /// @param text label to give to the setting
    /// @param decimals amount of decimals to show
    /// @param increment what to increment/decrement the value by
    /// @param currentValue the starting value
    /// @param minValue the minimum value the setting is allowed to be
    /// @param maxValue the maximum value the setting is allowed to be
    /// @param anchoredPosition the position of the setting
    /// @param onValueChange what to run when the value is changed
    /// @return the created increment setting
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, StringW text, int decimals, float increment, float currentValue, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    
    /// @brief Creates an incerement setting, meaning a float value with arrows to go up and down
    /// @param parent what to parent it to
    /// @param text label to give to the setting
    /// @param decimals amount of decimals to show
    /// @param increment what to increment/decrement the value by
    /// @param currentValue the starting value
    /// @param hasMin whether or not the minimum value should be enforced
    /// @param hasMax whether or not the maximum value should be enforced
    /// @param minValue the minimum value the setting is allowed to be
    /// @param maxValue the maximum value the setting is allowed to be
    /// @param anchoredPosition the position of the setting
    /// @param onValueChange what to run when the value is changed
    /// @return the created increment setting
    QuestUI::IncrementSetting* CreateIncrementSetting(UnityEngine::Transform* parent, StringW text, int decimals, float increment, float currentValue, bool hasMin, bool hasMax, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);

    /// @brief Overload for creating an increment setting that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the created increment setting
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline QuestUI::IncrementSetting* CreateIncrementSetting(T parent, TArgs...args) {
        return CreateIncrementSetting(parent->get_transform(), args...);
    }

    /// @brief Creates a slider setting
    /// @param parent what to parent it to
    /// @param text the text displayed in front to let the user know what they are editing
    /// @param increment the increment each "tick" should do
    /// @param value the initial value it should be
    /// @param minValue the minimum value for the slider
    /// @param maxValue the maximum value for the slider
    /// @param onValueChange callback called after the user has not interacted with the slider for 1 second
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW text, float increment, float value, float minValue, float maxValue, std::function<void(float)> onValueChange = nullptr);
    
    /// @brief Creates a slider setting
    /// @param parent what to parent it to
    /// @param text the text displayed in front to let the user know what they are editing
    /// @param increment the increment each "tick" should do
    /// @param value the initial value it should be
    /// @param minValue the minimum value for the slider
    /// @param maxValue the maximum value for the slider
    /// @param applyValueTime the amount of seconds to wait after the user stopped interacting to call the callback
    /// @param onValueChange callback called after the user has not interacted with the slider for 1 second
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW text, float increment, float value, float minValue, float maxValue, float applyValueTime, std::function<void(float)> onValueChange = nullptr);

    /// @brief Creates a slider setting
    /// @param parent what to parent it to
    /// @param text the text displayed in front to let the user know what they are editing
    /// @param increment the increment each "tick" should do
    /// @param value the initial value it should be
    /// @param minValue the minimum value for the slider
    /// @param maxValue the maximum value for the slider
    /// @param anchoredPosition the position
    /// @param onValueChange callback called after the user has not interacted with the slider for 1 second
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW text, float increment, float value, float minValue, float maxValue, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);
    
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
    QuestUI::SliderSetting* CreateSliderSetting(UnityEngine::Transform* parent, StringW text, float increment, float value, float minValue, float maxValue, float applyValueTime, UnityEngine::Vector2 anchoredPosition, std::function<void(float)> onValueChange = nullptr);

    /// @brief Overload for creating a slider setting that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the created slider setting
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline QuestUI::SliderSetting* CreateSliderSetting(T parent, TArgs...args) {
        return CreateSliderSetting(parent->get_transform(), args...);
    }

    /// @brief Creates a scroll view
    /// @param parent what to parent it to
    /// @return created container which you should add your content to
    UnityEngine::GameObject* CreateScrollView(UnityEngine::Transform* parent);

    /// @brief Overload for creating a scroll view that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return created container which you should add your content to
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::GameObject* CreateScrollView(T parent) {
        return CreateScrollView(parent->get_transform());
    }
    /// @brief Overload for creating a scrollable settings container that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return created container which you should add your content to
    UnityEngine::GameObject* CreateScrollableSettingsContainer(UnityEngine::Transform* parent);

    /// @brief Overload for creating a scrollable settings container that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return created container which you should add your content to
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::GameObject* CreateScrollableSettingsContainer(T parent) {
        return CreateScrollableSettingsContainer(parent->get_transform());
    }

    /// @brief creates a string setting allowing users to input a string with a keyboard
    /// @param parent what to parent it to
    /// @param settingsName the label to give to the setting
    /// @param currentValue the current string value it is
    /// @param onValueChange callback ran when the value changes
    /// @return the created string setting
    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, StringW settingsName, StringW currentValue, std::function<void(StringW)> onValueChange = nullptr);
    
    /// @brief creates a string setting allowing users to input a string with a keyboard
    /// @param parent what to parent it to
    /// @param settingsName the label to give to the setting
    /// @param currentValue the current string value it is
    /// @param anchoredPosition position of the string setting
    /// @param onValueChange callback ran when the value changes
    /// @return the created string setting
    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, StringW settingsName, StringW currentValue, UnityEngine::Vector2 anchoredPosition, std::function<void(StringW)> onValueChange = nullptr);
    
    /// @brief creates a string setting allowing users to input a string with a keyboard
    /// @param parent what to parent it to
    /// @param settingsName the label to give to the setting
    /// @param currentValue the current string value it is
    /// @param anchoredPosition position of the string setting
    /// @param keyboardPositionOffset offset of the keyboard in 3d space
    /// @param onValueChange callback ran when the value changes
    /// @return the created string setting
    HMUI::InputFieldView* CreateStringSetting(UnityEngine::Transform* parent, StringW settingsName, StringW currentValue, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector3 keyboardPositionOffset, std::function<void(StringW)> onValueChange = nullptr);

    /// @brief Overload for creating a string setting that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the created string setting
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline HMUI::InputFieldView* CreateStringSetting(T parent, TArgs...args) {
        return CreateStringSetting(parent->get_transform(), args...);
    }

    /// @brief creates a dropwdown menu to select from a set of pre-known strings (like an enum)
    /// @param parent what to parent it to
    /// @param dropdownName label of the setting
    /// @param currentValue what to display as currently selected
    /// @param values the possible string values that can be displayed
    /// @param onValueChange callback ran when the value changes
    /// @return the created dropdown
    HMUI::SimpleTextDropdown* CreateDropdown(UnityEngine::Transform* parent, StringW dropdownName, StringW currentValue, std::vector<StringW> values, std::function<void(StringW)> onValueChange = nullptr);

    /// @brief Overload for creating a dropdown menu that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @param dropdownName label of the setting
    /// @param currentValue what to display as currently selected
    /// @param values the possible string values that can be displayed
    /// @param onValueChange callback ran when the value changes
    /// @return the created dropdown
    template<HasTransform T>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline HMUI::SimpleTextDropdown* CreateDropdown(T parent, StringW dropdownName, StringW currentValue, std::vector<StringW> values, std::function<void(StringW)> onValueChange = nullptr) {
        return CreateDropdown(parent->get_transform(), dropdownName, currentValue, values, onValueChange);
    }

    /// @brief creates a floating screen
    /// @param screenSize the size of the screen
    /// @param position where to place it
    /// @param rotation how to rotate it
    /// @param curvatureRadius how curved is it?
    /// @param hasBackground does it have a background?
    /// @param createHandle should questui create a handle?
    /// @param handleSide which side should have a handle? 0 = left, 1 = right, 2 = bottom, 3 = top, 4 = all
    /// @return gameobject to parent your content to
    UnityEngine::GameObject* CreateFloatingScreen(UnityEngine::Vector2 screenSize, UnityEngine::Vector3 position, UnityEngine::Vector3 rotation, float curvatureRadius = 0, bool hasBackground = true, bool createHandle = true, int handleSide = 4);

    /// @brief creates a color picker
    /// @param parent what to parent it to
    /// @param text the label to display
    /// @param defaultColor the starting color
    /// @param onValueChange callback ran when the color is changed
    /// @return the created color picker object
    UnityEngine::GameObject* CreateColorPicker(UnityEngine::Transform* parent, StringW text, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType)> onValueChange = nullptr);
    
    /// @brief Overload for creating a color picker that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @param text the label to display
    /// @param defaultColor the starting color
    /// @param onValueChange callback ran when the color is changed
    /// @return the created color picker object
    template<HasTransform T>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline UnityEngine::GameObject* CreateColorPicker(T parent, StringW text, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color, GlobalNamespace::ColorChangeUIEventType)> onValueChange = nullptr) {
        return CreateColorPicker(parent->get_transform(), text, defaultColor, onValueChange);
    }

    /// @brief Creates a color picker modal
    /// @param parent what to parent it to
    /// @param name the text to display
    /// @param defaultColor the color to start off with
    /// @param onDone the callback to call when color picking is done
    /// @param onCancel the callback to call when the user cancels
    /// @param onChange the callback to call when the color changes at all
    QuestUI::ModalColorPicker* CreateColorPickerModal(UnityEngine::Transform* parent, StringW name, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color)> onDone = nullptr, std::function<void()> onCancel = nullptr, std::function<void(UnityEngine::Color)> onChange = nullptr);
    
    /// @brief Overload for creating a color picker modal that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @param text the label to display
    /// @param defaultColor the starting color
    /// @param onValueChange callback ran when the color is changed
    /// @return the created color picker modal object
    template<HasTransform T>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline QuestUI::ModalColorPicker* CreateColorPickerModal(T parent, StringW name, UnityEngine::Color defaultColor, std::function<void(UnityEngine::Color)> onDone = nullptr, std::function<void()> onCancel = nullptr, std::function<void(UnityEngine::Color)> onChange = nullptr) {
        return CreateColorPickerModal(parent->get_transform(), name, defaultColor, onDone, onCancel, onChange);
    }

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

    /// @brief Overload for creating a modal that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the created modal
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline HMUI::ModalView* CreateModal(T parent, TArgs...args) {
        return CreateModal(parent->get_transform(), args...);
    }

    /// @brief creates a scrollable container for the given modal, with exact fit
    /// @param modal the modal to create a container for
    /// @return GameObject container
    UnityEngine::GameObject* CreateScrollableModalContainer(HMUI::ModalView* modal);

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

    /// @brief Overload for creating a list that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return CustomListTableData* so you can add your data to the list
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline CustomListTableData* CreateList(T parent, TArgs...args) {
        return CreateList(parent->get_transform(), args...);
    }

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
    
    /// @brief Overload for creating a scrollable list that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return CustomListTableData* so you can add your data to the list
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline CustomListTableData* CreateScrollableList(T parent, TArgs...args) {
        return CreateScrollableList(parent->get_transform(), args...);
    }

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
    requires (std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
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
    requires (std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
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
    requires (std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateCustomSourceList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, anchoredPosition, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T, HasTransform U>
    requires (!std::is_convertible_v<U, UnityEngine::Transform*>, std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateCustomSourceList(U parent, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent->get_transform(), {0.0f, 0.0f}, {35.0f, 60.0f}, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T, HasTransform U>
    requires (!std::is_convertible_v<U, UnityEngine::Transform*>, std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateCustomSourceList(U parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent->get_transform(), {0.0f, 0.0f}, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T, HasTransform U>
    requires (!std::is_convertible_v<U, UnityEngine::Transform*>, std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateCustomSourceList(U parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent->get_transform(), anchoredPosition, sizeDelta, onCellWithIdxClicked);
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
    requires (std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
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
    requires (std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
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
    requires (std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateScrollableCustomSourceList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateScrollableCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent, anchoredPosition, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T, HasTransform U>
    requires (!std::is_convertible_v<U, UnityEngine::Transform*>, std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateScrollableCustomSourceList(U parent, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateScrollableCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent->get_transform(), {0.0f, 0.0f}, {35.0f, 60.0f}, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T, HasTransform U>
    requires (!std::is_convertible_v<U, UnityEngine::Transform*>, std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateScrollableCustomSourceList(U parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateScrollableCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent->get_transform(), {0.0f, 0.0f}, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a list with a custom data source passed in by the modder
    /// @tparam T class that implements HMUI::TableView::IDataSource & is a component (Monobehaviour)
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param onCellWithIdxClicked the callback called when a cell is clicked
    template<typename T, HasTransform U>
    requires (!std::is_convertible_v<U, UnityEngine::Transform*>, std::is_convertible_v<T, UnityEngine::MonoBehaviour*>)
    T CreateScrollableCustomSourceList(U parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked = nullptr)
    {
        return (T)CreateScrollableCustomSourceList(reinterpret_cast<System::Type*>(csTypeOf(T)), parent->get_transform(), anchoredPosition, sizeDelta, onCellWithIdxClicked);
    }

    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param anchoredPosition the position
    /// @param sizeDelta the sizeDelta
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    /// @return the created text segmented control
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, ArrayW<StringW> values, std::function<void(int)> onCellWithIdxClicked = nullptr);
    
    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param sizeDelta the sizeDelta
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    /// @return the created text segmented control
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, ArrayW<StringW> values, std::function<void(int)> onCellWithIdxClicked = nullptr);
    
    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param values list of text values to give to the controller
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    /// @return the created text segmented control
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, ArrayW<StringW> values, std::function<void(int)> onCellWithIdxClicked);

    /// @brief creates a text segmented control like the one on the gameplay setup view controller
    /// @param parent what to parent it to
    /// @param onCellWithIdxClicked callback called when a cell is clicked
    /// @return the created text segmented control
    QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked);
    
    /// @brief Overload for creating a text segmented control that allows you to pass in anything that has a ->get_transform() method for the parent
    /// @param parent what to parent it to
    /// @return the created text segmented control
    template<HasTransform T, typename ...TArgs>
    requires(!std::is_convertible_v<T, UnityEngine::Transform*>)
    inline QuestUI::CustomTextSegmentedControlData* CreateTextSegmentedControl(T parent, TArgs...args) {
        return CreateTextSegmentedControl(parent->get_transform(), args...);
    }
}
