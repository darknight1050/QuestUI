#include "CustomTypes/Components/GameplaySetup.hpp"

#include "BeatSaberUI.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"
#include "GlobalNamespace/SinglePlayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/GameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"

#include "GlobalNamespace/PlayerSettingsPanelController.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/MultiplayerSettingsPanelController.hpp"
#include "GlobalNamespace/ColorsOverrideSettingsPanelController.hpp"
#include "GlobalNamespace/EnvironmentOverrideSettingsPanelController.hpp"

#include "HMUI/TextSegmentedControl.hpp"
#include "Sprites/carats.hpp"

DEFINE_TYPE(QuestUI, GameplaySetup);

static Il2CppString* BaseGameplaySetupWrapper_cs = nullptr;
static Il2CppString* QuestuiGameplaySetupWrapper_cs = nullptr;

extern Logger& getLogger();

namespace QuestUI
{
    void GameplaySetup::ctor()
    {
        INVOKE_CTOR();
    }
    
    void GameplaySetup::Setup()
    {
        auto canvas = BeatSaberUI::CreateCanvas();
        canvas->get_transform()->SetParent(get_transform(), false);
        auto controlRect = reinterpret_cast<UnityEngine::RectTransform*>(canvas->get_transform());
        auto control = BeatSaberUI::CreateTextSegmentedControl(controlRect, {0, 0}, {80, 5.5}, {u"vanilla", u"mods"}, std::bind(&GameplaySetup::SwitchGameplayTab, this, std::placeholders::_1));
        auto layout = control->get_gameObject()->AddComponent<UnityEngine::UI::LayoutElement*>();
        layout->set_preferredWidth(100);

        controlRect->set_anchoredPosition({0, 45});
        controlRect->set_sizeDelta({80, 8.5});
        controlRect->set_localScale({1, 1, 1});
        
        // all the base game stuff will be on this
        auto baseGameRect = UnityEngine::GameObject::New_ctor()->AddComponent<UnityEngine::RectTransform*>();
        if (!BaseGameplaySetupWrapper_cs) BaseGameplaySetupWrapper_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BaseGameplaySetupWrapper");
        baseGameRect->get_gameObject()->set_name(BaseGameplaySetupWrapper_cs);
        baseGameRect->SetParent(get_transform(), false);
        get_transform()->GetChild(0)->SetParent(baseGameRect, true);
        auto self = GetComponent<GlobalNamespace::GameplaySetupViewController*>();
        self->selectionSegmentedControl->get_transform()->SetParent(baseGameRect, true);            
        self->playerSettingsPanelController->get_transform()->SetParent(baseGameRect, true);            
        self->gameplayModifiersPanelController->get_transform()->SetParent(baseGameRect, true);            
        self->multiplayerSettingsPanelController->get_transform()->SetParent(baseGameRect, true);            
        self->environmentOverrideSettingsPanelController->get_transform()->SetParent(baseGameRect, true);            
        self->colorsOverrideSettingsPanelController->get_transform()->SetParent(baseGameRect, true);        

        // all the custom stuff will live hereg
        auto vertical = BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        vertical->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        vertical->set_childForceExpandHeight(false);
        vertical->set_childForceExpandWidth(false);
        vertical->set_childControlHeight(false);
        
        auto customRect = vertical->get_rectTransform();
        
        if (!QuestuiGameplaySetupWrapper_cs) QuestuiGameplaySetupWrapper_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIGameplaySetupWrapper");
        customRect->get_gameObject()->set_name(QuestuiGameplaySetupWrapper_cs);
        customRect->get_transform()->SetParent(get_transform(), false);
        controlRect->SetAsFirstSibling();
        vertical->get_gameObject()->SetActive(false);
        auto horizontal = BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());
        horizontal->set_childForceExpandWidth(true);
        segmentedController = BeatSaberUI::CreateTextSegmentedControl(horizontal->get_transform(), {0, 0}, {80, 5.5}, {}, std::bind(&GameplaySetup::ChooseModSegment, this, std::placeholders::_1));
        segmentedController->get_gameObject()->GetComponent<UnityEngine::UI::HorizontalLayoutGroup*>()->set_childForceExpandWidth(true);
        float buttonSize = 8.0f;
        auto left = BeatSaberUI::CreateUIButton(horizontal->get_transform(), "", "SettingsButton", UnityEngine::Vector2(0, 0), UnityEngine::Vector2(buttonSize, buttonSize), std::bind(&GameplaySetup::MoveModMenus, this, -2));
        left->get_transform()->SetAsFirstSibling();
        auto right = BeatSaberUI::CreateUIButton(horizontal->get_transform(), "", "SettingsButton", UnityEngine::Vector2(0, 0), UnityEngine::Vector2(buttonSize, buttonSize), std::bind(&GameplaySetup::MoveModMenus, this, 2));
        right->get_transform()->SetAsLastSibling();

        reinterpret_cast<UnityEngine::RectTransform*>(left->get_transform()->GetChild(0))->set_sizeDelta({buttonSize, buttonSize});
        reinterpret_cast<UnityEngine::RectTransform*>(right->get_transform()->GetChild(0))->set_sizeDelta({buttonSize, buttonSize});
        auto carat_left_sprite = BeatSaberUI::Base64ToSprite(carat_left);
        auto carat_right_sprite = BeatSaberUI::Base64ToSprite(carat_right);
        auto carat_left_inactive_sprite = BeatSaberUI::Base64ToSprite(carat_left_inactive);
        auto carat_right_inactive_sprite = BeatSaberUI::Base64ToSprite(carat_right_inactive);
        BeatSaberUI::SetButtonSprites(left, carat_left_inactive_sprite, carat_left_sprite);
        BeatSaberUI::SetButtonSprites(right, carat_right_inactive_sprite, carat_right_sprite);
    }

    void GameplaySetup::MoveModMenus(int offset)
    {
        currentFirst += offset;
        int size = currentTabs.size();
        if (currentFirst < 0) currentFirst += size;
        if (currentFirst >= currentTabs.size()) currentFirst -= size;

        SetModTexts();
        ChooseModSegment(0);
    }

    void GameplaySetup::Activate(bool firstActivation)
    {
        getLogger().info("activate");
        if (firstActivation) Setup();

        currentTabs = GameplaySetupMenuTabs::get(GetMenuType());

        SetModTexts();

        if (currentTabs.size() > 0) 
        {
            currentMenu = currentTabs[currentFirst + segmentedController->segmentedControl->get_selectedCellNumber()];
            if (!currentMenu->gameObject) currentMenu->CreateObject(get_transform()->Find(QuestuiGameplaySetupWrapper_cs));
            currentMenu->Activate();
        }
    }

    Register::MenuType GameplaySetup::GetMenuType()
    {
        auto flowCoordinator = BeatSaberUI::GetMainFlowCoordinator()->YoungestChildFlowCoordinatorOrSelf();
        auto klass = il2cpp_functions::object_get_class(reinterpret_cast<Il2CppObject*>(flowCoordinator));

        if (il2cpp_functions::class_is_assignable_from(classof(GlobalNamespace::CampaignFlowCoordinator*), klass))
            return Register::MenuType::Campaign;
        else if (il2cpp_functions::class_is_assignable_from(classof(GlobalNamespace::SinglePlayerLevelSelectionFlowCoordinator*), klass))
            return Register::MenuType::Solo;
        else if (il2cpp_functions::class_is_assignable_from(classof(GlobalNamespace::GameServerLobbyFlowCoordinator*), klass))
            return Register::MenuType::Online;
        else return Register::MenuType::Custom;
    }

    void GameplaySetup::SwitchGameplayTab(int idx)
    {
        get_transform()->Find(BaseGameplaySetupWrapper_cs)->get_gameObject()->SetActive(idx == 0);
        get_transform()->Find(QuestuiGameplaySetupWrapper_cs)->get_gameObject()->SetActive(idx == 1);
    }

    void GameplaySetup::SetModTexts()
    {
        if (currentTabs.size() > tabCount)
        {
            int first = currentFirst;
            int size = currentTabs.size();
            std::vector<std::u16string> texts;
            for (int i = 0; i < tabCount; i++) {
                int current = first + i;
                while (current >= size) current -= size;
                texts.push_back(to_utf16(currentTabs[current]->title));
            }

            segmentedController->set_texts(texts);
        }
        else
        {
            std::vector<std::u16string> texts;
            for (auto t : currentTabs)
            {
                texts.push_back(to_utf16(t->title));
            }
            segmentedController->set_texts(texts);
        }
        segmentedController->segmentedControl->SelectCellWithNumber(0);
    }

    void GameplaySetup::ChooseModSegment(int idx)
    {
        int index = currentFirst + idx;
        if (index >= currentTabs.size()) index = 0;
        auto nextMenu = currentTabs[index];
        if (currentMenu == nextMenu) return;
        currentMenu->Deactivate();
        if (!nextMenu->gameObject) nextMenu->CreateObject(get_transform()->Find(QuestuiGameplaySetupWrapper_cs));
        nextMenu->Activate();
        currentMenu = nextMenu;
    }
}