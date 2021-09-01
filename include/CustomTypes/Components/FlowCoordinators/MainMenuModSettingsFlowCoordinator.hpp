#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"
#include "CustomTypes/Components/ViewControllers/MainMenuModSettingsPlaceholderViewController.hpp"

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
        ModSettingsInfos::ModSettingsInfo* currentInfo = nullptr;
        //void MyReplaceTopViewController(HMUI::ViewController* viewController, HMUI::FlowCoordinator* originalOwnerFlowCoordinator, HMUI::FlowCoordinator* newOwnerFlowCoordinator, System::Action* finishedCallback = nullptr, HMUI::ViewController::AnimationType animationType = HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection animationDirection = HMUI::ViewController::AnimationDirection::Horizontal);
        custom_types::Helpers::Coroutine EndOfFramePresentVC();
)
