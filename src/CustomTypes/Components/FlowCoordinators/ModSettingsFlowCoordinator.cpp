#include "CustomTypes/Components/FlowCoordinators/ModSettingsFlowCoordinator.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Data/ModSettingsButtonClickData.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "System/Action_1.hpp"

#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::ModSettingsFlowCoordinator);

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;
using namespace TMPro;

void OnOpenModSettings(QuestUI::ModSettingsFlowCoordinator* self, QuestUI::ModSettingsButtonClickData* data) {
    QuestUI::ModSettingsInfos::ModSettingsInfo* info = (QuestUI::ModSettingsInfos::ModSettingsInfo*)data->info;
    if(info->viewController) {
        self->SetTitle(il2cpp_utils::createcsstr(info->title), ViewController::AnimationType::In);
        self->ReplaceTopViewController(info->viewController, self, self, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
        self->ActiveViewController = info->viewController;
    } else if(info->flowCoordinator) {
        self->PresentFlowCoordinator(info->flowCoordinator, nullptr, ViewController::AnimationDirection::Horizontal, false, false);
    }
}

void QuestUI::ModSettingsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        SetTitle(il2cpp_utils::createcsstr("Mod Settings"), ViewController::AnimationType::Out);
        showBackButton = true;
        for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            switch(info.type) {
                case Register::Type::VIEW_CONTROLLER: {
                    info.viewController = BeatSaberUI::CreateViewController(info.il2cpp_type);
                    if(info.showModInfo){
                        VerticalLayoutGroup* layout = BeatSaberUI::CreateVerticalLayoutGroup(info.viewController->get_rectTransform());
                        layout->get_rectTransform()->set_anchoredPosition(UnityEngine::Vector2(0.0f, -48.0f));
                        GameObject* layoutGameObject = layout->get_gameObject();
                        layoutGameObject->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
                        layoutGameObject->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
                        layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));
                        TextMeshProUGUI* modInfoText = BeatSaberUI::CreateText(layout->get_transform(), info.modInfo.id + "|v" + info.modInfo.version);
                        modInfoText->set_alignment(TextAlignmentOptions::Center);
                        modInfoText->set_fontSize(4.8f);
                    }
                    break;
                }
                case Register::Type::FLOW_COORDINATOR: {
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

void QuestUI::ModSettingsFlowCoordinator::BackButtonWasPressed(ViewController* topViewController) {
    if (ActiveViewController != ModSettingsButtonsViewController){
        SetTitle(il2cpp_utils::createcsstr("Mod Settings"), ViewController::AnimationType::Out);
        ReplaceTopViewController(ModSettingsButtonsViewController, this, this, nullptr, ViewController::AnimationType::Out, ViewController::AnimationDirection::Horizontal);
        ActiveViewController = ModSettingsButtonsViewController;
    }else {
        BeatSaberUI::getMainFlowCoordinator()->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
    }
}