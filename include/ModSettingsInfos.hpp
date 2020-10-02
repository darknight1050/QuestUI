#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"

namespace QuestUI::ModSettingsInfos {
    
    typedef struct ModSettingsInfo {
        ModInfo modInfo;
        Il2CppReflectionType* viewControllerType;
        HMUI::ViewController* viewController;
    } ModSettingsInfo;

    std::vector<ModSettingsInfo>& get();
    void add(ModSettingsInfo info);
}

