#pragma once
#include "ModSettingsInfos.hpp"

#include "CustomTypes/Components/ViewControllers/ModSettingsButtonsViewController.hpp"
#include "custom-types/shared/macros.hpp"

namespace QuestUI {

    typedef struct ModSettingsButtonClickData {
        QuestUI::ModSettingsButtonsViewController* viewController;
        QuestUI::ModSettingsInfos::ModSettingsInfo& info;
    } ModSettingsButtonClickData;
    
}
