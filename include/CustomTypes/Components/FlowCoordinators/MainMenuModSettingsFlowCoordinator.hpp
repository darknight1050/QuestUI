#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"

namespace QuestUI { 
    namespace ModSettingsInfos {
        struct ModSettingsInfo;
    }
}

DECLARE_CLASS_CODEGEN(QuestUI, MainMenuModSettingsFlowCoordinator, HMUI::FlowCoordinator,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::FlowCoordinator::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::FlowCoordinator::BackButtonWasPressed>::get(), HMUI::ViewController* topViewController);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, placeholder);
    public:
        static MainMenuModSettingsFlowCoordinator* get_instance();
        ModSettingsInfos::ModSettingsInfo* currentInfo = nullptr;
        custom_types::Helpers::Coroutine EndOfFramePresentVC();
    private:
        static SafePtrUnity<MainMenuModSettingsFlowCoordinator> instance;
)
