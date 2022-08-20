#include <functional>
#include "ModSettingsInfos.hpp"
#include "customlogger.hpp"

#include "BSML/shared/BSML.hpp"
#include "BeatSaberUI.hpp"
#include "CustomTypes/Components/FlowCoordinators/MainMenuModSettingsFlowCoordinator.hpp"
#include "CustomTypes/Data/SettingsHost.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"

using namespace QuestUI::ModSettingsInfos;

std::vector<ModSettingsInfo> modSettingsInfos;

std::vector<ModSettingsInfo>& QuestUI::ModSettingsInfos::get() {
    return modSettingsInfos;
}

inline QuestUI::Register::MenuLocation operator&(const QuestUI::Register::MenuLocation& lhs, const QuestUI::Register::MenuLocation& rhs) {
    return static_cast<QuestUI::Register::MenuLocation>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

void QuestUI::ModSettingsInfos::add(ModSettingsInfo info) {
    getLogger().info("Added ModSettingsInfo(%s|v%s, %d)", info.modInfo.id.c_str(), info.modInfo.version.c_str(), info.type);
    modSettingsInfos.push_back(info);
    
    if ((info.location & QuestUI::Register::MenuLocation::MainMenu) == QuestUI::Register::MenuLocation::MainMenu) {
        BSML::Register::RegisterMenuButton(info.title, info.modInfo.id, 
        [idx = modSettingsInfos.size() - 1](){
            auto fc = BeatSaberUI::GetMainFlowCoordinator()->YoungestChildFlowCoordinatorOrSelf();
            auto& info = modSettingsInfos[idx];

            if (info.type == QuestUI::Register::Type::FLOW_COORDINATOR) {
                if (!info.flowCoordinator || !info.flowCoordinator->m_CachedPtr.m_value) {
                    info.flowCoordinator = BeatSaberUI::CreateFlowCoordinator(info.il2cpp_type);
                }

                fc->PresentFlowCoordinator(info.flowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
            } else if (info.type == QuestUI::Register::Type::VIEW_CONTROLLER) {
                if (!info.viewController || !info.viewController->m_CachedPtr.m_value) {
                    info.viewController = BeatSaberUI::CreateViewController(info.il2cpp_type);
                }

                auto modSettingsFlowCoordinator = QuestUI::MainMenuModSettingsFlowCoordinator::get_instance();
                modSettingsFlowCoordinator->currentInfo = &info;
                fc->PresentFlowCoordinator(modSettingsFlowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, true, false);
            }
        });

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