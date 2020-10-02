#include "CustomTypes/ViewControllers/ModSettingsButtonsViewController.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/Events/UnityAction.hpp"

#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::ModSettingsButtonsViewController);

DEFINE_EVENT(QuestUI::ModSettingsButtonsViewController, System::Action_1<HMUI::ViewController*>*, openViewController);

struct ModSettingsButtonClickData {
    QuestUI::ModSettingsButtonsViewController* viewController;
    QuestUI::ModSettingsInfos::ModSettingsInfo& info;
};

void ModSettingsButtonClick(ModSettingsButtonClickData* data, UnityEngine::UI::Button* button){
    getLogger().info("ModSettingsButtonClick %s", data->info.modInfo.id.c_str());
    if(data->viewController->openViewController)
        data->viewController->openViewController->Invoke(data->info.viewController);
}

void QuestUI::ModSettingsButtonsViewController::DidActivate(bool firstActivation, HMUI::ViewController::ActivationType activationType) {
    if(activationType == HMUI::ViewController::ActivationType::AddedToHierarchy && firstActivation) {
        for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            BeatSaberUI::CreateUIButton(get_rectTransform(), "OkButton", UnityEngine::Vector2(0.0f, -2.0f), UnityEngine::Vector2(28.0f, 10.0f), il2cpp_utils::MakeAction<UnityEngine::Events::UnityAction>(il2cpp_functions::class_get_type(classof(UnityEngine::Events::UnityAction*)), new ModSettingsButtonClickData{ this, info }, ModSettingsButtonClick), info.modInfo.id, nullptr);
        }
    }
}
