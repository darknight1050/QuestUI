#include "CustomTypes/Components/ViewControllers/MainMenuModSettingsViewController.hpp"

#include "MainMenuModSettingInfos.hpp"
#include "BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"

#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(QuestUI, MainMenuModSettingsViewController);

extern UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent);
extern Logger& getLogger();

TMPro::TextMeshProUGUI* AddHeader(UnityEngine::Transform* parent, std::string_view title, UnityEngine::Vector2 size, UnityEngine::Vector2 anchoredPosition, UnityEngine::Color leftColor, UnityEngine::Color rightColor)
{
    using namespace QuestUI::BeatSaberUI;
	auto vertical = CreateVerticalLayoutGroup(parent);
	vertical->get_rectTransform()->set_anchoredPosition(anchoredPosition);
	auto horizontal = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());


	auto text = CreateText(horizontal->get_transform(), std::string(title), false);
	text->set_fontSize(text->get_fontSize() * 2.0f);
	text->set_alignment(TMPro::TextAlignmentOptions::Center);


	auto layoutelem = text->get_gameObject()->AddComponent<UnityEngine::UI::LayoutElement*>();
	layoutelem->set_preferredHeight(size.y);
	layoutelem->set_preferredWidth(size.x);


	auto background = horizontal->get_gameObject()->AddComponent<QuestUI::Backgroundable*>();
	background->ApplyBackgroundWithAlpha(il2cpp_utils::createcsstr("title-gradient"), 1.0f);

	auto imageView = background->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>();
	imageView->gradient = true;
	imageView->gradientDirection = 0;
	imageView->set_color({1.0f, 1.0f, 1.0f, 1.0f});
	imageView->set_color0(leftColor);
	imageView->set_color1(rightColor);
	imageView->skew = 0.18f;
	imageView->curvedCanvasSettingsHelper->Reset();
	return text;
}
    
namespace QuestUI
{
    void MainMenuModSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        getLogger().info("main menu mod settings present");
        if (!firstActivation) return;
        for (auto& info : MainMenuModSettingInfos::get()) {
            info.viewController = nullptr;
            info.flowCoordinator = nullptr;
        }
        float grayness = 0.5f;
        AddHeader(get_transform(), "Mods", {50, 10}, {0, 45}, {grayness, grayness, grayness, 1.0f}, {grayness, grayness, grayness, 0.0f});
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* scrollView = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = scrollView->GetComponent<ExternalComponents*>();
        UnityEngine::RectTransform* scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(12.0f, 0.0f));
        UnityEngine::UI::HorizontalLayoutGroup* layoutGroup = nullptr;
        std::vector<ModSettingsInfos::ModSettingsInfo>& infos = MainMenuModSettingInfos::get();
        for(int i = 0; i < infos.size(); i++) {
            ModSettingsInfos::ModSettingsInfo& info = infos[i];
            if(i % 3 == 0)
                layoutGroup = CreateHorizontalLayoutGroup(scrollView->get_transform());
            BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), info.modInfo.id, UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(36.0f, 10.0f), 
                [this, &info] {
                    getLogger().info("OnModSettingsButtonClick %s", info.modInfo.id.c_str());
                    this->OnOpenModSettings(&info);
                }
            );
        }
        getLogger().info("presented!");
    }

    void MainMenuModSettingsViewController::OnOpenModSettings(ModSettingsInfos::ModSettingsInfo* info)
    {
        auto currentFlowCoordinator = BeatSaberUI::GetMainFlowCoordinator()->YoungestChildFlowCoordinatorOrSelf();
        switch (info->type)
        {
            case Register::Type::VIEW_CONTROLLER: {
                getLogger().info("View Controller, FC: %p", modSettingsFlowCoordinator);
                if (!modSettingsFlowCoordinator)
                    modSettingsFlowCoordinator = BeatSaberUI::CreateFlowCoordinator<MainMenuModSettingsFlowCoordinator*>();
                modSettingsFlowCoordinator->currentInfo = info;
                currentFlowCoordinator->PresentFlowCoordinator(modSettingsFlowCoordinator, nullptr, ViewController::AnimationDirection::Horizontal, true, false);
                break;
            }
            case Register::Type::FLOW_COORDINATOR: {
                getLogger().info("Flow Coordinator");
                if(!info->flowCoordinator)
                    info->flowCoordinator = BeatSaberUI::CreateFlowCoordinator(info->il2cpp_type);
                currentFlowCoordinator->PresentFlowCoordinator(info->flowCoordinator, nullptr, ViewController::AnimationDirection::Horizontal, false, false);
                break;
            }
        }
    }
}