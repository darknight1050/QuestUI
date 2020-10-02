#pragma once

#include "modloader/shared/modloader.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"

namespace QuestUI {

    void Init();

    class Register {

        private:
            static void RegisterModSettings(ModInfo modInfo, Il2CppReflectionType* viewController);

        public:
            template<class T = HMUI::ViewController*>
            static void RegisterModSettings(ModInfo modInfo){
                RegisterModSettings(modInfo, typeof(T));
            }

    };

}