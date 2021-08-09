#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/CustomPanelController.hpp"

#include "customlogger.hpp"

DEFINE_TYPE(QuestUI, CustomPanelController);

void QuestUI::CustomPanelController::Refresh() {
    getLogger().info("CustomPanelController Refresh");
}
