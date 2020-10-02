#pragma once

#include "ModSettingsInfos.hpp"

#include "CustomTypes/ModSettingsButtonClickData.hpp"

#include "UnityEngine/GameObject.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_ActivationType.hpp"
#include "System/Action_1.hpp"

#include "custom-types/shared/macros.hpp"

#include "eventmacros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ModSettingsButtonsViewController, HMUI::ViewController,

    DECLARE_EVENT(System::Action_1<QuestUI::ModSettingsButtonClickData*>*, openModSettings);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 2), bool firstActivation, HMUI::ViewController::ActivationType activationType);

    REGISTER_FUNCTION(ModSettingsButtonsViewController,
        REGISTER_EVENT(openModSettings);

        REGISTER_METHOD(DidActivate);
    )
)