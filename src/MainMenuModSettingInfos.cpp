#include "MainMenuModSettingInfos.hpp"

extern Logger& getLogger();

namespace QuestUI::MainMenuModSettingInfos {
    std::vector<QuestUI::ModSettingsInfos::ModSettingsInfo> mainMenuModSettingsInfos;

    std::vector<QuestUI::ModSettingsInfos::ModSettingsInfo>& get()
    {
        return mainMenuModSettingsInfos;
    }

    void add(QuestUI::ModSettingsInfos::ModSettingsInfo& info)
    {
        getLogger().info("Added MainMenuModSettingsInfo(%s|v%s, %d)", info.modInfo.id.c_str(), info.modInfo.version.c_str(), info.type);
        mainMenuModSettingsInfos.push_back(info);
    }
}