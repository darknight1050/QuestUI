#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "QuestUI.hpp"

namespace QuestUI::ModSettingsInfos {

    struct ModSettingsInfo {
        modloader::ModInfo modInfo =  { "", "", 0 };
        bool showModInfo;
        std::string title;
        Register::Type type;
        System::Type* il2cpp_type;
        union {
            HMUI::ViewController* viewController;
            HMUI::FlowCoordinator* flowCoordinator;
        };
        Register::DidActivateEvent didActivateEvent;
        Register::MenuLocation location;
        void Present();
    };

    std::vector<ModSettingsInfo>& get();
    void add(ModSettingsInfo info);
}

