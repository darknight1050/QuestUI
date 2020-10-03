
#include "CustomTypes/ModSettingsButtonClickData.hpp"

DEFINE_CLASS(QuestUI::ModSettingsButtonClickData);

void QuestUI::ModSettingsButtonClickData::ctor(HMUI::ViewController* viewController, void* info) {
    this->viewController = viewController;
    this->info = info;
}
