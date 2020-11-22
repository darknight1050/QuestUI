#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Components/ViewControllers/ModSettingsButtonsViewController.hpp"
#include "ModSettingsButtonClickData.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/TextAnchor.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "HMUI/Touchable.hpp"
#include "BeatSaberUI.hpp"

#include "customlogger.hpp"

DEFINE_CLASS(QuestUI::ModSettingsButtonsViewController);

DEFINE_EVENT(QuestUI::ModSettingsButtonsViewController, System::Action_1<QuestUI::CustomDataType*>*, openModSettings);

void OnModSettingsButtonClick(QuestUI::CustomDataType* data, UnityEngine::UI::Button* button) {
    QuestUI::ModSettingsButtonClickData& clickData = data->GetData<QuestUI::ModSettingsButtonClickData>();
    getLogger().info("OnModSettingsButtonClick %s", clickData.info.modInfo.id.c_str());
    if(clickData.viewController->openModSettings)
        clickData.viewController->openModSettings->Invoke(data);
}

void QuestUI::ModSettingsButtonsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(addedToHierarchy && firstActivation) {
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::UI::GridLayoutGroup* layoutGroup = BeatSaberUI::CreateGridLayoutGroup(get_transform());
        layoutGroup->set_constraint(UnityEngine::UI::GridLayoutGroup::Constraint::Flexible);
        layoutGroup->set_cellSize(UnityEngine::Vector2(48.0f, 10.0f));
        layoutGroup->set_spacing(UnityEngine::Vector2(3.0f, 3.0f));
        layoutGroup->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);
        UnityEngine::RectTransform* rectTransform = layoutGroup->GetComponent<UnityEngine::RectTransform*>();
        for(ModSettingsInfos::ModSettingsInfo& info : ModSettingsInfos::get()) {
            BeatSaberUI::CreateUIButton(rectTransform, info.modInfo.id, il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), CRASH_UNLESS(il2cpp_utils::New<CustomDataType*>(classof(CustomDataType*)))->SetData(ModSettingsButtonClickData{this, info}), OnModSettingsButtonClick));
        }
    }
}
