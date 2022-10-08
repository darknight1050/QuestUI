#include <functional>
#include "ModSettingsInfos.hpp"
#include "customlogger.hpp"

#include "bsml/shared/BSML.hpp"
#include "BeatSaberUI.hpp"
#include "CustomTypes/Components/FlowCoordinators/MainMenuModSettingsFlowCoordinator.hpp"
#include "CustomTypes/Data/SettingsHost.hpp"

#include "custom-types/shared/delegate.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_DidActivateDelegate.hpp"

using namespace QuestUI::ModSettingsInfos;

std::vector<ModSettingsInfo> modSettingsInfos;

std::vector<ModSettingsInfo>& QuestUI::ModSettingsInfos::get() {
    return modSettingsInfos;
}

inline QuestUI::Register::MenuLocation operator&(const QuestUI::Register::MenuLocation& lhs, const QuestUI::Register::MenuLocation& rhs) {
    return static_cast<QuestUI::Register::MenuLocation>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

void QuestUI::ModSettingsInfos::ModSettingsInfo::Present() {
    auto fc = BeatSaberUI::GetMainFlowCoordinator()->YoungestChildFlowCoordinatorOrSelf();

    if (type == QuestUI::Register::Type::FLOW_COORDINATOR) {
        if (!flowCoordinator || !flowCoordinator->m_CachedPtr.m_value) {
            flowCoordinator = BeatSaberUI::CreateFlowCoordinator(il2cpp_type);
        }

        fc->PresentFlowCoordinator(flowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
    } else if (type == QuestUI::Register::Type::VIEW_CONTROLLER) {
        if (!viewController || !viewController->m_CachedPtr.m_value) {
            viewController = BeatSaberUI::CreateViewController(il2cpp_type);
            if (didActivateEvent) {
                std::function<void(bool, bool, bool)> fun = 
                [vc = viewController, mptr = didActivateEvent]
                (bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
                    mptr(vc, firstActivation, addedToHierarchy, screenSystemEnabling);
                };
                auto didActivateDelegate = custom_types::MakeDelegate<HMUI::ViewController::DidActivateDelegate*>(fun);
                viewController->add_didActivateEvent(didActivateDelegate);
            }
        }

        auto modSettingsFlowCoordinator = QuestUI::MainMenuModSettingsFlowCoordinator::get_instance();
        modSettingsFlowCoordinator->currentInfo = this;
        fc->PresentFlowCoordinator(modSettingsFlowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, true, false);
    }
}


void QuestUI::ModSettingsInfos::add(ModSettingsInfo info) {
    getLogger().info("Added ModSettingsInfo(%s|v%s, %d)", info.modInfo.id.c_str(), info.modInfo.version.c_str(), info.type);
    modSettingsInfos.push_back(info);
    
    if ((info.location & QuestUI::Register::MenuLocation::MainMenu) == QuestUI::Register::MenuLocation::MainMenu) {
        BSML::Register::RegisterMenuButton(info.title, info.modInfo.id,
            [idx = modSettingsInfos.size() - 1](){
                ModSettingsInfos::get()[idx].Present();
            }
        );
    }

    if ((info.location & QuestUI::Register::MenuLocation::Settings) == QuestUI::Register::MenuLocation::Settings) {
        SettingsHost::get_instance()->AddSubSetting(modSettingsInfos.size() - 1);
        static bool registered = false;
        if (!registered) {
            registered = true;
            BSML::Register::RegisterSettingsMenu("QuestUI", MOD_ID "_settings", SettingsHost::get_instance(), false);
        }
    }
}