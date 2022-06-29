#include "CustomTypes/Components/ViewControllers/MainMenuModSettingsViewController.hpp"
#include "ModSettingsInfos.hpp"

#include "BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"

#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(QuestUI, MainMenuModSettingsViewController);

extern UnityEngine::UI::HorizontalLayoutGroup* CreateHorizontalLayoutGroup(UnityEngine::Transform* parent);
extern Logger& getLogger();

TMPro::TextMeshProUGUI* AddHeader(UnityEngine::Transform* parent, std::string_view title, UnityEngine::Vector2 size, UnityEngine::Vector2 anchoredPosition, UnityEngine::Color leftColor, UnityEngine::Color rightColor, float fontSize)
{
    using namespace QuestUI::BeatSaberUI;
	auto vertical = CreateVerticalLayoutGroup(parent);
	vertical->get_rectTransform()->set_anchoredPosition(anchoredPosition);
	auto horizontal = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());


	auto text = CreateText(horizontal->get_transform(), std::string(title), false);
	text->set_fontSize(fontSize);
	text->set_alignment(TMPro::TextAlignmentOptions::Center);


	auto layoutelem = text->get_gameObject()->AddComponent<UnityEngine::UI::LayoutElement*>();
    
	layoutelem->set_preferredHeight(size.y);
	layoutelem->set_preferredWidth(size.x);


	auto background = horizontal->get_gameObject()->AddComponent<QuestUI::Backgroundable*>();
    static ConstString backgroundName("title-gradient");
	background->ApplyBackgroundWithAlpha(backgroundName, 1.0f);

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
        getLogger().info("MainMenuModSettingsViewController DidActivate %d %d", firstActivation, addedToHierarchy); 
        if (!firstActivation) return;

        float blackness = 0.0f;
        AddHeader(get_transform(), "Mods", {35, 14}, {-7.5, 30}, {blackness, blackness, blackness, 0.5f}, {blackness, blackness, blackness, 0.5f}, 10.0f);
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* scrollView = BeatSaberUI::CreateScrollView(get_transform());
        ExternalComponents* externalComponents = scrollView->GetComponent<ExternalComponents*>();
        
        UnityEngine::RectTransform* scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
        scrollTransform->set_anchoredPosition(UnityEngine::Vector2(-7.5f, -20.0f));
        scrollTransform->set_sizeDelta(UnityEngine::Vector2(23.0f, 0.0f));
        scrollTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.3f));
        UnityEngine::UI::HorizontalLayoutGroup* layoutGroup = nullptr;
        std::vector<ModSettingsInfos::ModSettingsInfo>& infos = ModSettingsInfos::get();     
        int currentItems = 0;   
        for(int i = 0; i < infos.size(); i++) {
            ModSettingsInfos::ModSettingsInfo& info = infos[i];
            if(info.location == Register::MenuLocation::AllViews || info.location == Register::MenuLocation::MainMenu){
                if(currentItems % 3 == 0)
                    layoutGroup = CreateHorizontalLayoutGroup(scrollView->get_transform());
                    auto modButton = BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), info.title, UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(40.0f, 10.0f), 
                    [this, &info] {
                        getLogger().info("OnMainMenuModSettingsButtonClick %s", info.modInfo.id.c_str());
                        this->OnOpenModSettings(info);
                    });
                currentItems++;
            }
            
        }
        //creates extra hidden buttons which forces a left align, because for some reason set_childAlignment(UnityEngine::TextAnchor::MiddleLeft) doesnt work for what ive tried

        if((currentItems % 3) > 0)
            for(int i = 0; i < ((currentItems % 3) - 3) * -1; i++){
                auto emptyLeftAlignButton = BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), "", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(40.0f, 10.0f), nullptr);
                emptyLeftAlignButton->get_transform()->Find("BG")->get_gameObject()->SetActive(false);
                emptyLeftAlignButton->get_transform()->Find("Underline")->get_gameObject()->SetActive(false);
        }
    }

    void MainMenuModSettingsViewController::OnOpenModSettings(ModSettingsInfos::ModSettingsInfo& info)
    {
        auto currentFlowCoordinator = BeatSaberUI::GetMainFlowCoordinator()->YoungestChildFlowCoordinatorOrSelf();
        switch (info.type)
        {
            case Register::Type::VIEW_CONTROLLER: {
                getLogger().info("View Controller, FC: %p", modSettingsFlowCoordinator);
                if (!modSettingsFlowCoordinator)
                    modSettingsFlowCoordinator = BeatSaberUI::CreateFlowCoordinator<MainMenuModSettingsFlowCoordinator*>();
                modSettingsFlowCoordinator->currentInfo = &info;
                currentFlowCoordinator->PresentFlowCoordinator(modSettingsFlowCoordinator, nullptr, ViewController::AnimationDirection::Horizontal, true, false);
                break;
            }
            case Register::Type::FLOW_COORDINATOR: {
                getLogger().info("Flow Coordinator");
                if(!info.flowCoordinator)
                    info.flowCoordinator = BeatSaberUI::CreateFlowCoordinator(info.il2cpp_type);
                currentFlowCoordinator->PresentFlowCoordinator(info.flowCoordinator, nullptr, ViewController::AnimationDirection::Horizontal, false, false);
                break;
            }
        }
    }
}