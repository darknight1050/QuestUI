#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/CustomPanelController.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::CustomPanelController);

void QuestUI::CustomPanelController::Refresh() {
    getLogger().info("CustomPanelController Refresh");
}
