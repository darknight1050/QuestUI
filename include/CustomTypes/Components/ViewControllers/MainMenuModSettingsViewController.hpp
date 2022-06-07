#pragma once

#include "CustomTypes/Components/FlowCoordinators/MainMenuModSettingsFlowCoordinator.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

#include <functional>

namespace QuestUI { 
    namespace ModSettingsInfos {
        struct ModSettingsInfo;
    }
}

DECLARE_CLASS_CODEGEN(QuestUI, MainMenuModSettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(QuestUI::MainMenuModSettingsFlowCoordinator*, modSettingsFlowCoordinator);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::ViewController::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    void OnOpenModSettings(ModSettingsInfos::ModSettingsInfo& info);
)