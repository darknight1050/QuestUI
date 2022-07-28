#include "CustomTypes/Components/FlowCoordinators/ModSettingsFlowCoordinator.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"

#include "custom-types/shared/delegate.hpp"

#include "ModSettingsInfos.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_DidActivateDelegate.hpp"
#include "System/Action_1.hpp"

#include "BeatSaberUI.hpp"
#include "InternalBeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_TYPE(QuestUI, ModSettingsFlowCoordinator);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;
using namespace TMPro;

void QuestUI::ModSettingsFlowCoordinator::OnOpenModSettings(ModSettingsInfos::ModSettingsInfo& info) {
    switch(info.type) {
        case Register::Type::VIEW_CONTROLLER: {
            if(!info.viewController) {
                info.viewController = BeatSaberUI::CreateViewController(info.il2cpp_type);
                if(info.showModInfo)
                    BeatSaberUI::AddModInfoText(info);
                if(info.didActivateEvent)
                    info.viewController->add_didActivateEvent(il2cpp_utils::MakeDelegate<ViewController::DidActivateDelegate*>(info.viewController, info.didActivateEvent));
            }
            SetTitle(info.title, ViewController::AnimationType::In);
            ReplaceTopViewController(info.viewController, this, this, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
            ActiveViewController = info.viewController;
            break;
        }
        case Register::Type::FLOW_COORDINATOR: {
            if(!info.flowCoordinator)
                info.flowCoordinator = BeatSaberUI::CreateFlowCoordinator(info.il2cpp_type);
            PresentFlowCoordinator(info.flowCoordinator, nullptr, ViewController::AnimationDirection::Horizontal, false, false);
            break;
        }
        default:
            break;
    }
}

void QuestUI::ModSettingsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        static ConstString modSettingsName("Mod Settings");
        SetTitle(modSettingsName, ViewController::AnimationType::Out);
        showBackButton = true;
        if(!ModSettingsButtonsViewController)
            ModSettingsButtonsViewController = BeatSaberUI::CreateViewController<QuestUI::ModSettingsButtonsViewController*>();
        ModSettingsButtonsViewController->OnOpenModSettings = [this](ModSettingsInfos::ModSettingsInfo& info) { this->OnOpenModSettings(info); };
        ProvideInitialViewControllers(ModSettingsButtonsViewController, nullptr, nullptr, nullptr, nullptr);
        ActiveViewController = ModSettingsButtonsViewController;
    }
}

void QuestUI::ModSettingsFlowCoordinator::BackButtonWasPressed(ViewController* topViewController) {
    if (ActiveViewController != ModSettingsButtonsViewController) {
        static ConstString modSettingsName("Mod Settings");
        SetTitle(modSettingsName, ViewController::AnimationType::Out);
        ReplaceTopViewController(ModSettingsButtonsViewController, this, this, nullptr, ViewController::AnimationType::Out, ViewController::AnimationDirection::Horizontal);
        ActiveViewController = ModSettingsButtonsViewController;
    } else {
        parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
    }
}