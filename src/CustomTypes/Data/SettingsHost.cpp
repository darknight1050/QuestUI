#include "CustomTypes/Data/SettingsHost.hpp"

#include "BeatSaberUI.hpp"
#include "CustomTypes/Components/FlowCoordinators/MainMenuModSettingsFlowCoordinator.hpp"

#include "custom-types/shared/delegate.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_DidActivateDelegate.hpp"

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
        ModSettingsInfos::get()[index].Present();
    }
}