#pragma once

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"

namespace QuestUI {

    void Init();

    int GetModsCount();

    HMUI::FlowCoordinator* getModSettingsFlowCoordinator();

    class Register {
        public:
            enum Type {
                VIEW_CONTROLLER,
                FLOW_COORDINATOR,
            };

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo, std::string title, bool showModInfo = true){
                RegisterModSettings(modInfo, showModInfo, title, typeof(T), Type::VIEW_CONTROLLER);
            }

            template<class T = HMUI::ViewController*>
            static void RegisterModSettingsViewController(ModInfo modInfo) {
                RegisterModSettingsViewController<T>(modInfo, modInfo.id);
            }
            
            template<class T = HMUI::FlowCoordinator*>
            static void RegisterModSettingsFlowCoordinator(ModInfo modInfo) {
                RegisterModSettings(modInfo, false, modInfo.id, typeof(T), Type::FLOW_COORDINATOR);
            }

        private:
            static void RegisterModSettings(ModInfo modInfo, bool showModInfo, std::string title, Il2CppReflectionType* il2cpp_type, Type type);

    };

}