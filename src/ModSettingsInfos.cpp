#include <functional>
#include "ModSettingsInfos.hpp"
#include "customlogger.hpp"

using namespace QuestUI::ModSettingsInfos;

std::vector<ModSettingsInfo> modSettingsInfos;

std::vector<ModSettingsInfo>& QuestUI::ModSettingsInfos::get() {
    return modSettingsInfos;
}

void QuestUI::ModSettingsInfos::add(ModSettingsInfo info) {
    getLogger().info("Added ModSettingsInfo(%s|v%s, %d)", info.modInfo.id.c_str(), info.modInfo.version.c_str(), info.type);
    modSettingsInfos.push_back(info);
}