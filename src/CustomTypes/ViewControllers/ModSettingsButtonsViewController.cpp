#include "CustomTypes/ViewControllers/ModSettingsButtonsViewController.hpp"
#include "CustomTypes/ModSettingsButtonClickData.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/UI/GridLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::ModSettingsButtonsViewController);

DEFINE_EVENT(QuestUI::ModSettingsButtonsViewController, System::Action_1<QuestUI::ModSettingsButtonClickData*>*, openModSettings);

void OnModSettingsButtonClick(QuestUI::ModSettingsButtonClickData* data, UnityEngine::UI::Button* button) {
    QuestUI::ModSettingsInfos::ModSettingsInfo* info = (QuestUI::ModSettingsInfos::ModSettingsInfo*)data->info;
    QuestUI::ModSettingsButtonsViewController* viewController = (QuestUI::ModSettingsButtonsViewController*)data->viewController;
    getLogger().info("OnModSettingsButtonClick %s", info->modInfo.id.c_str());
    if(viewController->openModSettings){
        viewController->openModSettings->Invoke(data);
    }
        
}

void QuestUI::ModSettingsButtonsViewController::DidActivate(bool firstActivation, HMUI::ViewController::ActivationType activationType) {
    if(activationType == HMUI::ViewController::ActivationType::AddedToHierarchy && firstActivation) {
		UnityEngine::RectTransform* layoutTransform = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("ModSettingsButtonsGridLayoutGroup"), typeof(UnityEngine::UI::GridLayoutGroup*))->GetComponent<UnityEngine::RectTransform*>();
		layoutTransform->SetParent(get_transform(), false);
		layoutTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
		layoutTransform->set_sizeDelta(UnityEngine::Vector2(150.0f, 74.0f));
        UnityEngine::UI::GridLayoutGroup* layoutGroup = layoutTransform->GetComponent<UnityEngine::UI::GridLayoutGroup*>();
        layoutGroup->set_constraint(UnityEngine::UI::GridLayoutGroup::Constraint::Flexible);
        layoutGroup->set_cellSize(UnityEngine::Vector2(48.0f, 10.0f));
        layoutGroup->set_spacing(UnityEngine::Vector2(3.0f, 3.0f));
		for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            BeatSaberUI::CreateUIButton(layoutTransform, "OkButton", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(0.0f, 0.0f), il2cpp_utils::MakeAction<UnityEngine::Events::UnityAction>(il2cpp_functions::class_get_type(classof(UnityEngine::Events::UnityAction*)), *il2cpp_utils::New<QuestUI::ModSettingsButtonClickData*>(classof(QuestUI::ModSettingsButtonClickData*), this, (void*)&info), OnModSettingsButtonClick), info.modInfo.id, nullptr);
        }
    }
}
