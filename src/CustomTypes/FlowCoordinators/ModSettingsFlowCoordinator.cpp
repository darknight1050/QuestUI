#include "CustomTypes/FlowCoordinators/ModSettingsFlowCoordinator.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/Object.hpp"
#include "HMUI/ViewController_SlideAnimationDirection.hpp"
#include "System/Action_1.hpp"

#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::ModSettingsFlowCoordinator);

void OnOpenViewController(QuestUI::ModSettingsFlowCoordinator* self, HMUI::ViewController* viewController) {
    getLogger().info("OnOpenViewController %p", viewController);
    if(viewController){
        self->ReplaceTopViewController(viewController, self, self, nullptr, false, HMUI::ViewController::SlideAnimationDirection::Right);
        self->ActiveViewController = viewController;
    }
}

void QuestUI::ModSettingsFlowCoordinator::DidActivate(bool firstActivation, HMUI::FlowCoordinator::ActivationType activationType) {
    if(activationType == HMUI::ViewController::ActivationType::AddedToHierarchy && firstActivation) {
        set_title(il2cpp_utils::createcsstr("Mod Settings"));
        showBackButton = true;
        for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            if(!info.viewController)
                info.viewController = BeatSaberUI::CreateViewController(info.viewControllerType);
        }
        if(!ModSettingsButtonsViewController)
        ModSettingsButtonsViewController = BeatSaberUI::CreateViewController<QuestUI::ModSettingsButtonsViewController*>();
        ModSettingsButtonsViewController->add_openViewController(il2cpp_utils::MakeAction<System::Action_1<HMUI::ViewController*>>(il2cpp_functions::class_get_type(classof(System::Action_1<HMUI::ViewController*>*)), this, OnOpenViewController));
        ProvideInitialViewControllers(ModSettingsButtonsViewController, nullptr, nullptr, nullptr, nullptr);
        ActiveViewController = ModSettingsButtonsViewController;
    }
}

void QuestUI::ModSettingsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
    if (ActiveViewController != ModSettingsButtonsViewController){
        ReplaceTopViewController(ModSettingsButtonsViewController, this, this, nullptr, false, HMUI::ViewController::SlideAnimationDirection::Left);
        ActiveViewController = ModSettingsButtonsViewController;
    }else {
        BeatSaberUI::getMainFlowCoordinator()->DismissFlowCoordinator(this, nullptr, false);
    }
}