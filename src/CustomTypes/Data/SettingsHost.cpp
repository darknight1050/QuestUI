#include "CustomTypes/Data/SettingsHost.hpp"

#include "BeatSaberUI.hpp"
#include "CustomTypes/Components/FlowCoordinators/MainMenuModSettingsFlowCoordinator.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"

DEFINE_TYPE(QuestUI, SettingsHost);
DEFINE_TYPE(QuestUI, SubSettingsHost);

namespace QuestUI {
    SafePtr<SettingsHost> SettingsHost::instance;
    SettingsHost* SettingsHost::get_instance() {
        if (!instance) {
            instance = SettingsHost::New_ctor();
        }
        return instance.ptr();
    }

    ListWrapper<Il2CppObject*> SettingsHost::get_settings() {
        if (!_settings) {
            _settings = List<Il2CppObject*>::New_ctor();
        }
        return _settings;
    }

    bool SettingsHost::get_anySettings() {
        return get_settings().size() > 0;
    }

    void SettingsHost::AddSubSetting(int index) {
        get_settings()->Add(SubSettingsHost::New_ctor(index));
    }

    void SubSettingsHost::ctor(int index) {
        this->index = index;
    }

    StringW SubSettingsHost::get_title() {
        return ModSettingsInfos::get()[index].title;
    }
    
    StringW SubSettingsHost::get_message() {
        return "Open the menu for " + ModSettingsInfos::get()[index].title;
    }
    
    void SubSettingsHost::Open() {
        auto fc = BeatSaberUI::GetMainFlowCoordinator()->YoungestChildFlowCoordinatorOrSelf();
        auto& info = ModSettingsInfos::get()[index];

        if (info.type == Register::Type::FLOW_COORDINATOR) {
            if (!info.flowCoordinator || !info.flowCoordinator->m_CachedPtr.m_value) {
                info.flowCoordinator = BeatSaberUI::CreateFlowCoordinator(info.il2cpp_type);
            }

            fc->PresentFlowCoordinator(info.flowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
        } else {
            if (!info.viewController || !info.viewController->m_CachedPtr.m_value) {
                info.viewController = BeatSaberUI::CreateViewController(info.il2cpp_type);
            }

            auto modSettingsFlowCoordinator = QuestUI::MainMenuModSettingsFlowCoordinator::get_instance();
            modSettingsFlowCoordinator->currentInfo = &info;
            fc->PresentFlowCoordinator(modSettingsFlowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, true, false);
        }
    }
}