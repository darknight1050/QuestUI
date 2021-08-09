#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "QuestUI.hpp"

namespace QuestUI::ModSettingsInfos {
    typedef struct ModSettingsInfo {
        ModInfo modInfo;
        bool showModInfo;
        std::string title;
        Register::Type type;
        System::Type* il2cpp_type;
        HMUI::ViewController* viewController;
        HMUI::FlowCoordinator* flowCoordinator;
        Register::DidActivateEvent didActivateEvent;
    } ModSettingsInfo;

    std::vector<ModSettingsInfo>& get();
    void add(ModSettingsInfo info);
}

