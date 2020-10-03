#include "CustomTypes/FlowCoordinators/ModSettingsFlowCoordinator.hpp"
#include "CustomTypes/ModSettingsButtonClickData.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/Object.hpp"
#include "HMUI/ViewController_SlideAnimationDirection.hpp"
#include "System/Action_1.hpp"

#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::ModSettingsFlowCoordinator);

void OnOpenModSettings(QuestUI::ModSettingsFlowCoordinator* self, QuestUI::ModSettingsButtonClickData* data) {
    QuestUI::ModSettingsInfos::ModSettingsInfo* info = (QuestUI::ModSettingsInfos::ModSettingsInfo*)data->info;
    if(info->viewController) {
        self->set_title(il2cpp_utils::createcsstr(info->title));
        self->ReplaceTopViewController(info->viewController, self, self, nullptr, false, HMUI::ViewController::SlideAnimationDirection::Right);
        self->ActiveViewController = info->viewController;
    } else if(info->flowCoordinator) {
        self->PresentFlowCoordinator(info->flowCoordinator, nullptr, false, false);
    }
}

void QuestUI::ModSettingsFlowCoordinator::DidActivate(bool firstActivation, HMUI::FlowCoordinator::ActivationType activationType) {
    if(activationType == HMUI::ViewController::ActivationType::AddedToHierarchy && firstActivation) {
        set_title(il2cpp_utils::createcsstr("Mod Settings"));
        showBackButton = true;
        for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            switch(info.type) {
                case Register::Type::VIEW_CONTROLLER: {
                    if(!info.viewController) {
                        info.viewController = BeatSaberUI::CreateViewController(info.il2cpp_type);
                        BeatSaberUI::CreateText(info.viewController->get_transform(), "v" + info.modInfo.version, UnityEngine::Vector2(44.0f, -32.0f))->set_alignment(TMPro::TextAlignmentOptions::BottomRight);
                    }
                    break;
                }
                case Register::Type::FLOW_COORDINATOR: {
                    if(!info.flowCoordinator)
                        info.flowCoordinator = BeatSaberUI::CreateFlowCoordinator(info.il2cpp_type);
                    break;
                }
                default:
                break;
            }
            
        }
        if(!ModSettingsButtonsViewController)
            ModSettingsButtonsViewController = BeatSaberUI::CreateViewController<QuestUI::ModSettingsButtonsViewController*>();
        ModSettingsButtonsViewController->add_openModSettings(il2cpp_utils::MakeAction<System::Action_1<QuestUI::ModSettingsButtonClickData*>>(il2cpp_functions::class_get_type(classof(System::Action_1<QuestUI::ModSettingsButtonClickData*>*)), this, OnOpenModSettings));
        ProvideInitialViewControllers(ModSettingsButtonsViewController, nullptr, nullptr, nullptr, nullptr);
        ActiveViewController = ModSettingsButtonsViewController;
    }
}

void QuestUI::ModSettingsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
    if (ActiveViewController != ModSettingsButtonsViewController){
        set_title(il2cpp_utils::createcsstr("Mod Settings"));
        ReplaceTopViewController(ModSettingsButtonsViewController, this, this, nullptr, false, HMUI::ViewController::SlideAnimationDirection::Left);
        ActiveViewController = ModSettingsButtonsViewController;
    }else {
        BeatSaberUI::getMainFlowCoordinator()->DismissFlowCoordinator(this, nullptr, false);
    }
}