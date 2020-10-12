#pragma once

#include "modloader/shared/modloader.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"

namespace QuestUI {

    void Init();

    HMUI::FlowCoordinator* getModSettingsFlowCoordinator();

    class Register {
        public:
            enum Type {
                VIEW_CONTROLLER,
                FLOW_COORDINATOR,
            };

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, std::string title){
                RegisterModSettings(modInfo, title, typeof(T), Type::VIEW_CONTROLLER);
            }

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo) {
                RegisterModSettingsViewController<T>(modInfo, modInfo.id);
            }
            
            template<class T = HMUI::FlowCoordinator*>
            static void RegisterModSettingsFlowCoordinator(ModInfo modInfo) {
                RegisterModSettings(modInfo, modInfo.id, typeof(T), Type::FLOW_COORDINATOR);
            }

        private:
            static void RegisterModSettings(ModInfo modInfo, std::string title, Il2CppReflectionType* il2cpp_type, Type type);

    };

}