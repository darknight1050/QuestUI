#include "ModSettingsInfos.hpp"

namespace QuestUI::MainMenuModSettingInfos {
    std::vector<QuestUI::ModSettingsInfos::ModSettingsInfo>& get();
    void add(QuestUI::ModSettingsInfos::ModSettingsInfo& info);
}