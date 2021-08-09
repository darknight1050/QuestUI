#include "CustomTypes/Components/FlowCoordinators/ModSettingsFlowCoordinator.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_DidActivateDelegate.hpp"
#include "System/Action_1.hpp"

#include "BeatSaberUI.hpp"

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
                if(info.showModInfo) {
                    VerticalLayoutGroup* layout = BeatSaberUI::CreateVerticalLayoutGroup(info.viewController->get_rectTransform());
                    layout->get_rectTransform()->set_position(UnityEngine::Vector3(0.0f, -39.0f, -24.0f));
                    layout->get_rectTransform()->set_eulerAngles(UnityEngine::Vector3(60.0f, 0.0f, 0.0f));
                    GameObject* layoutGameObject = layout->get_gameObject();
                    layoutGameObject->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
                    layoutGameObject->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
                    layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 4, 2, 2));
                    TextMeshProUGUI* modInfoText = BeatSaberUI::CreateText(layout->get_transform(), info.modInfo.id + " : v" + info.modInfo.version);
                    modInfoText->set_alignment(TextAlignmentOptions::Center);
                    modInfoText->set_fontSize(4.8f);
                }
                if(info.didActivateEvent)
                    info.viewController->add_didActivateEvent(il2cpp_utils::MakeDelegate<ViewController::DidActivateDelegate*>(classof(ViewController::DidActivateDelegate*), info.viewController, info.didActivateEvent));
            }
            SetTitle(il2cpp_utils::createcsstr(info.title), ViewController::AnimationType::In);
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
        for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            info.viewController = nullptr;
            info.flowCoordinator = nullptr;
        }
        SetTitle(il2cpp_utils::createcsstr("Mod Settings"), ViewController::AnimationType::Out);
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
        SetTitle(il2cpp_utils::createcsstr("Mod Settings"), ViewController::AnimationType::Out);
        ReplaceTopViewController(ModSettingsButtonsViewController, this, this, nullptr, ViewController::AnimationType::Out, ViewController::AnimationDirection::Horizontal);
        ActiveViewController = ModSettingsButtonsViewController;
    } else {
        parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
    }
}