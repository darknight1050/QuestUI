#pragma once

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"

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

            typedef void(*DidActivateEvent)(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, std::string title, bool showModInfo = true, DidActivateEvent didActivateEvent = nullptr){
                RegisterModSettings(modInfo, showModInfo, title, csTypeOf(T), Type::VIEW_CONTROLLER, didActivateEvent);
            }
            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, std::string title, DidActivateEvent didActivateEvent){
                RegisterModSettingsViewController<T>(modInfo, title, true, didActivateEvent);
            }

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, DidActivateEvent didActivateEvent = nullptr) {
                RegisterModSettingsViewController<T>(modInfo, modInfo.id, true, didActivateEvent);
            }
            
            template<class T = HMUI::FlowCoordinator*>
            static void RegisterModSettingsFlowCoordinator(ModInfo modInfo) {
                RegisterModSettings(modInfo, false, modInfo.id, csTypeOf(T), Type::FLOW_COORDINATOR);
            }

        private:
            static void RegisterModSettings(ModInfo modInfo, bool showModInfo, std::string title, Il2CppReflectionType* il2cpp_type, Type type, DidActivateEvent didActivateEvent = nullptr);

    };

}