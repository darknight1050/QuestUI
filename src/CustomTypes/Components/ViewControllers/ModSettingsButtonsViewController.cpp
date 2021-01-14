#include "CustomTypes/Components/ViewControllers/ModSettingsButtonsViewController.hpp"
#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "ModSettingsButtonClickData.hpp"

#include "ModSettingsInfos.hpp"

#include "UnityEngine/TextAnchor.hpp"
#include "UnityEngine/RectTransform.hpp"
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

UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent){
    UnityEngine::UI::HorizontalLayoutGroup* layoutGroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(parent);
    layoutGroup->set_childForceExpandHeight(false);
    layoutGroup->set_childForceExpandWidth(false);
    layoutGroup->set_childControlHeight(true);
    layoutGroup->set_childControlWidth(true);
    layoutGroup->set_childAlignment(UnityEngine::TextAnchor::MiddleLeft);
    layoutGroup->set_spacing(3.0f);
    return layoutGroup;
}

void QuestUI::ModSettingsButtonsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(addedToHierarchy && firstActivation) {
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* scrollView = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = scrollView->GetComponent<ExternalComponents*>();
        UnityEngine::RectTransform* scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(-28.0f, 0.0f));
        UnityEngine::UI::HorizontalLayoutGroup* layoutGroup = nullptr;
        std::vector<ModSettingsInfos::ModSettingsInfo>& infos = ModSettingsInfos::get();
        for(int i = 0; i < infos.size(); i++) {
            ModSettingsInfos::ModSettingsInfo& info = infos[i];
            if(i % 3 == 0)
                layoutGroup = CreateHorizontalLayoutGroup(scrollView->get_transform());
            CustomDataType* data = CRASH_UNLESS(il2cpp_utils::New<CustomDataType*>(classof(CustomDataType*)))->SetData(ModSettingsButtonClickData{this, info});
            UnityEngine::Events::UnityAction* onClick = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), data, OnModSettingsButtonClick);
            BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), info.modInfo.id, UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(36.0f, 8.0f), onClick);
        }
    }
}
