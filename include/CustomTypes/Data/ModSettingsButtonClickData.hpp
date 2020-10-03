#pragma once
#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ModSettingsButtonClickData, Il2CppObject,


    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, viewController);
    DECLARE_INSTANCE_FIELD(void*, info);

    DECLARE_CTOR(ctor, HMUI::ViewController* viewController, void* info);
    
    REGISTER_FUNCTION(ModSettingsButtonClickData,
        REGISTER_FIELD(viewController);
        REGISTER_FIELD(info);

        REGISTER_METHOD(ctor);
    )
)