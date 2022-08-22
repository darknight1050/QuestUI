#include "CustomTypes/Components/FlowCoordinators/MainMenuModSettingsFlowCoordinator.hpp"
#include "custom-types/shared/delegate.hpp"

#include "ModSettingsInfos.hpp"
#include "BeatSaberUI.hpp"
#include "InternalBeatSaberUI.hpp"

#include "UnityEngine/WaitForEndOfFrame.hpp"
#include "UnityEngine/WaitForSeconds.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_DidActivateDelegate.hpp"

DEFINE_TYPE(QuestUI, MainMenuModSettingsFlowCoordinator);
extern Logger& getLogger();

namespace QuestUI
{
    SafePtrUnity<MainMenuModSettingsFlowCoordinator> MainMenuModSettingsFlowCoordinator::instance;
    MainMenuModSettingsFlowCoordinator* MainMenuModSettingsFlowCoordinator::get_instance() {
        if (!instance) {
            instance = BeatSaberUI::CreateFlowCoordinator<MainMenuModSettingsFlowCoordinator*>();
        }
        return instance.ptr();
    }

    void MainMenuModSettingsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        getLogger().info("MainMenuModSettingsFlowCoordinator activation");
        if (firstActivation)
        {
            getLogger().info("First activation");
            showBackButton = true;
            SetTitle("PlaceHolder", HMUI::ViewController::AnimationType::Out);
            placeholder = BeatSaberUI::CreateViewController<HMUI::ViewController*>();
            ProvideInitialViewControllers(placeholder, nullptr, nullptr, nullptr, nullptr);
        }

        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(EndOfFramePresentVC()));
    }

    void MainMenuModSettingsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
        parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    }

    custom_types::Helpers::Coroutine MainMenuModSettingsFlowCoordinator::EndOfFramePresentVC()
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());
        
        if (currentInfo)
        {
            if (!currentInfo->viewController) 
            {
                currentInfo->viewController = BeatSaberUI::CreateViewController(currentInfo->il2cpp_type);
                if(currentInfo->showModInfo)
                    BeatSaberUI::AddModInfoText(*currentInfo);
                if(currentInfo->didActivateEvent)
                    currentInfo->viewController->add_didActivateEvent(il2cpp_utils::MakeDelegate<HMUI::ViewController::DidActivateDelegate*>(currentInfo->viewController, currentInfo->didActivateEvent));
            }
            SetTitle(currentInfo->title, HMUI::ViewController::AnimationType::Out);
            ReplaceTopViewController(currentInfo->viewController, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        }

        co_return;
    }
}