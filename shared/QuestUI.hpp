#pragma once

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"

#include <concepts>

namespace QuestUI {

    void Init();

    bool DidInit();

    int GetModsCount();

    HMUI::FlowCoordinator* GetModSettingsFlowCoordinator();

    class Register {
        public:
            enum Type {
                VIEW_CONTROLLER,
                FLOW_COORDINATOR,
            };

            enum MenuType {
                Solo = 1,
                Online = 2,
                Campaign = 4,
                Custom = 8,
                All = Solo | Online | Campaign | Custom
            };

            typedef void(*DidActivateEvent)(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
            typedef void(*GameplaySetupMenuEvent)(UnityEngine::GameObject* self, bool firstActivation);

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, std::string title, bool showModInfo = true, DidActivateEvent didActivateEvent = nullptr) {
                static_assert(std::is_convertible<T, HMUI::ViewController*>());
                RegisterModSettings(modInfo, showModInfo, title, csTypeOf(T), Type::VIEW_CONTROLLER, didActivateEvent);
            }
            
            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, std::string title, DidActivateEvent didActivateEvent) {
                static_assert(std::is_convertible<T, HMUI::ViewController*>());
                RegisterModSettingsViewController<T>(modInfo, title, true, didActivateEvent);
            }

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, DidActivateEvent didActivateEvent = nullptr) {
                static_assert(std::is_convertible<T, HMUI::ViewController*>());
                RegisterModSettingsViewController<T>(modInfo, modInfo.id, true, didActivateEvent);
            }
            
            template<class T>
            static void RegisterModSettingsFlowCoordinator(ModInfo modInfo) {
                static_assert(std::is_convertible<T, HMUI::FlowCoordinator*>());
                RegisterModSettings(modInfo, false, modInfo.id, csTypeOf(T), Type::FLOW_COORDINATOR);
            }
        
            template<class T>
            static void RegisterModSettingsFlowCoordinator(ModInfo modInfo, std::string title, bool showModInfo = false) {
                static_assert(std::is_convertible<T, HMUI::FlowCoordinator*>());
                RegisterModSettings(modInfo, showModInfo, title, csTypeOf(T), Type::FLOW_COORDINATOR);
            }

            template<class T = HMUI::ViewController*>
            static void RegisterMainMenuModSettingsViewController(ModInfo modInfo, std::string title, bool showModInfo = true, DidActivateEvent didActivateEvent = nullptr) {
                static_assert(std::is_convertible<T, HMUI::ViewController*>());
                RegisterMainMenuModSettings(modInfo, showModInfo, title, csTypeOf(T), Type::VIEW_CONTROLLER, didActivateEvent);
            }
            
            template<class T = HMUI::ViewController*>
            static void RegisterMainMenuModSettingsViewController(ModInfo modInfo, std::string title, DidActivateEvent didActivateEvent) {
                static_assert(std::is_convertible<T, HMUI::ViewController*>());
                RegisterMainMenuModSettingsViewController<T>(modInfo, title, true, didActivateEvent);
            }

            template<class T = HMUI::ViewController*>
            static void RegisterMainMenuModSettingsViewController(ModInfo modInfo, DidActivateEvent didActivateEvent = nullptr) {
                static_assert(std::is_convertible<T, HMUI::ViewController*>());
                RegisterMainMenuModSettingsViewController<T>(modInfo, modInfo.id, true, didActivateEvent);
            }
            
            template<class T>
            static void RegisterMainMenuModSettingsFlowCoordinator(ModInfo modInfo) {
                static_assert(std::is_convertible<T, HMUI::FlowCoordinator*>());
                RegisterMainMenuModSettings(modInfo, false, modInfo.id, csTypeOf(T), Type::FLOW_COORDINATOR);
            }
        
            template<class T>
            static void RegisterMainMenuModSettingsFlowCoordinator(ModInfo modInfo, std::string title, bool showModInfo = false) {
                static_assert(std::is_convertible<T, HMUI::FlowCoordinator*>());
                RegisterMainMenuModSettings(modInfo, showModInfo, title, csTypeOf(T), Type::FLOW_COORDINATOR);
            }

            /// @brief Register a GameplaySetupMenu type for usage within the left menu
            /// @tparam T the type to register
            /// @param modInfo the modinfo used for the register
            /// @param menuType when to actually present this type
            template<class T>
            static void RegisterGameplaySetupMenu(ModInfo modInfo, int menuType = MenuType::All) {
                static_assert(std::is_convertible<T, UnityEngine::Component*>(), "Passed Type is not convertible to UnityEngine::Component!");
                RegisterGameplaySetupMenu(modInfo, modInfo.id, csTypeOf(T), menuType, nullptr);
            }

            /// @brief Register a GameplaySetupMenu type for usage within the left menu
            /// @tparam T the type to register
            /// @param modInfo the modinfo used for the register
            /// @param title the title used for display
            /// @param menuType when to actually present this type
            template<class T>
            static void RegisterGameplaySetupMenu(ModInfo modInfo, std::string_view title, int menuType = MenuType::All) {
                static_assert(std::is_convertible<T, UnityEngine::Component*>(), "Passed Type is not convertible to UnityEngine::Component!");
                RegisterGameplaySetupMenu(modInfo, title, csTypeOf(T), menuType, nullptr);
            }

            /// @brief Register a GameplaySetupMenu type for usage within the left menu
            /// @param modInfo the modinfo used for the register
            /// @param title the title used for display
            /// @param menuType when to actually present this type
            /// @param setupEvent callback called when your object is constructed
            static void RegisterGameplaySetupMenu(ModInfo modInfo, int menuType = MenuType::All, GameplaySetupMenuEvent setupEvent = nullptr) {
                RegisterGameplaySetupMenu(modInfo, modInfo.id, nullptr, menuType, setupEvent);
            }

            /// @brief Register a GameplaySetupMenu type for usage within the left menu
            /// @param modInfo the modinfo used for the register
            /// @param title the title used for display
            /// @param menuType when to actually present this type, MenuType bitmask
            /// @param setupEvent callback called when your object is constructed
            static void RegisterGameplaySetupMenu(ModInfo modInfo, std::string_view title, int menuType = MenuType::All, GameplaySetupMenuEvent setupEvent = nullptr) {
                RegisterGameplaySetupMenu(modInfo, title, nullptr, menuType, setupEvent);
            }

        private:
            static void RegisterModSettings(ModInfo modInfo, bool showModInfo, std::string title, Il2CppReflectionType* il2cpp_type, Type type, DidActivateEvent didActivateEvent = nullptr);
            static void RegisterMainMenuModSettings(ModInfo modInfo, bool showModInfo, std::string title, Il2CppReflectionType* il2cpp_type, Type type, DidActivateEvent didActivateEvent = nullptr);
            static void RegisterGameplaySetupMenu(ModInfo modInfo, std::string_view title, Il2CppReflectionType* il2cpp_type, int type, GameplaySetupMenuEvent setupEvent = nullptr);

    };

}
