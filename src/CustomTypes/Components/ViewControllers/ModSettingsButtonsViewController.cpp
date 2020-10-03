#include "CustomTypes/Components/ViewControllers/ModSettingsButtonsViewController.hpp"
#include "CustomTypes/Data/ModSettingsButtonClickData.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::ModSettingsButtonsViewController);

DEFINE_EVENT(QuestUI::ModSettingsButtonsViewController, System::Action_1<QuestUI::ModSettingsButtonClickData*>*, openModSettings);

void OnModSettingsButtonClick(QuestUI::ModSettingsButtonClickData* data, UnityEngine::UI::Button* button) {
    QuestUI::ModSettingsInfos::ModSettingsInfo* info = (QuestUI::ModSettingsInfos::ModSettingsInfo*)data->info;
    QuestUI::ModSettingsButtonsViewController* viewController = (QuestUI::ModSettingsButtonsViewController*)data->viewController;
    getLogger().info("OnModSettingsButtonClick %s", info->modInfo.id.c_str());
    if(viewController->openModSettings)
        viewController->openModSettings->Invoke(data);
}

void QuestUI::ModSettingsButtonsViewController::DidActivate(bool firstActivation, HMUI::ViewController::ActivationType activationType) {
    if(activationType == HMUI::ViewController::ActivationType::AddedToHierarchy && firstActivation) {
        UnityEngine::UI::GridLayoutGroup* layoutGroup = BeatSaberUI::CreateGridLayoutGroup(get_transform());
        layoutGroup->set_constraint(UnityEngine::UI::GridLayoutGroup::Constraint::Flexible);
        layoutGroup->set_cellSize(UnityEngine::Vector2(48.0f, 10.0f));
        layoutGroup->set_spacing(UnityEngine::Vector2(3.0f, 3.0f));
        UnityEngine::RectTransform* rectTransform = layoutGroup->GetComponent<UnityEngine::RectTransform*>();
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(5.0f, -2.5f));
        for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            BeatSaberUI::CreateUIButton(rectTransform, "OkButton", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(0.0f, 0.0f), il2cpp_utils::MakeAction<UnityEngine::Events::UnityAction>(il2cpp_functions::class_get_type(classof(UnityEngine::Events::UnityAction*)), *il2cpp_utils::New<QuestUI::ModSettingsButtonClickData*>(classof(QuestUI::ModSettingsButtonClickData*), this, (void*)&info), OnModSettingsButtonClick), info.modInfo.id, nullptr);
        }
    }
}
