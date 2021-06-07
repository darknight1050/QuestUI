#pragma once

#include "ModSettingsInfos.hpp"

#include "UnityEngine/GameObject.hpp"
#include "HMUI/ViewController.hpp"
#include "System/Action_1.hpp"

#include "custom-types/shared/macros.hpp"

#include "eventmacros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ModSettingsButtonsViewController, HMUI::ViewController,

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

    public:
        std::function<void(ModSettingsInfos::ModSettingsInfo&)> OnOpenModSettings;

)