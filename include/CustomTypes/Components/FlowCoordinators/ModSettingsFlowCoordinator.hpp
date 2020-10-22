#pragma once
#include "HMUI/ViewController.hpp"
#include "HMUI/NavigationController.hpp"
#include "HMUI/FlowCoordinator.hpp"

#include "CustomTypes/Components/ViewControllers/ModSettingsButtonsViewController.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ModSettingsFlowCoordinator, HMUI::FlowCoordinator,

    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, ActiveViewController);
    DECLARE_INSTANCE_FIELD(QuestUI::ModSettingsButtonsViewController*, ModSettingsButtonsViewController);
    

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);

    REGISTER_FUNCTION(ModSettingsFlowCoordinator,
        REGISTER_FIELD(ActiveViewController);
        REGISTER_FIELD(ModSettingsButtonsViewController);

        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(BackButtonWasPressed);
    )
)