#include "beatsaber-hook/shared/utils/utils.h"
#include "custom-types/shared/register.hpp"

#include "QuestUI.hpp"
#include "ModSettingsInfos.hpp"
#include "CustomTypes/Data/CustomDataType.hpp"
#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Components/CustomPanelController.hpp"
#include "CustomTypes/Components/ScrollViewContent.hpp"
#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "CustomTypes/Components/FlowCoordinators/ModSettingsFlowCoordinator.hpp"

#include "BeatSaberUI.hpp"
#include "InternalBeatSaberUI.hpp"
#include "Sprites/ModSettingsButton.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/GameplaySetupViewController_Panel.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/PlayerSettingsPanelController.hpp"
#include "GlobalNamespace/EnvironmentOverrideSettingsPanelController.hpp"
#include "GlobalNamespace/ColorsOverrideSettingsPanelController.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "Polyglot/Localization.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "System/Collections/Generic/IReadOnlyList_1.hpp"

#include "customlogger.hpp"

using namespace QuestUI;

Logger& getLogger() {
    static auto logger = new Logger(ModInfo{"questui", VERSION}, LoggerOptions(false, false));
    return *logger;
}

ModSettingsFlowCoordinator* flowCoordinator = nullptr;

HMUI::FlowCoordinator* QuestUI::GetModSettingsFlowCoordinator() {
    return flowCoordinator;
}

int QuestUI::GetModsCount() {
    return ModSettingsInfos::get().size();
}

void OnMenuModSettingsButtonClick(UnityEngine::UI::Button* button) {
    getLogger().info("MenuModSettingsButtonClick");
    if(!flowCoordinator)
        flowCoordinator = BeatSaberUI::CreateFlowCoordinator<ModSettingsFlowCoordinator*>();
    BeatSaberUI::GetMainFlowCoordinator()->PresentFlowCoordinator(flowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
}

MAKE_HOOK_OFFSETLESS(OptionsViewController_DidActivate, void, GlobalNamespace::OptionsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    OptionsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if(firstActivation) {
        flowCoordinator = nullptr;
        BeatSaberUI::ClearCache();
        if(GetModsCount() > 0) {
            UnityEngine::UI::Button* avatarButton = self->settingsButton;
            UnityEngine::UI::Button* button = UnityEngine::Object::Instantiate(avatarButton);
            static auto modSettingsStr = il2cpp_utils::createcsstr("Mod Settings", il2cpp_utils::StringType::Manual);
            button->set_name(modSettingsStr);
            static auto wrapperStr = il2cpp_utils::createcsstr("Wrapper", il2cpp_utils::StringType::Manual);
            UnityEngine::Transform* AvatarParent = self->get_transform()->Find(wrapperStr);
            button->get_transform()->SetParent(AvatarParent, false);
            button->get_transform()->SetAsFirstSibling();
            button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnMenuModSettingsButtonClick));
            
            UnityEngine::Object::Destroy(button->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>());

            button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->SetText(modSettingsStr);
            HMUI::ButtonSpriteSwap* spriteSwap = button->get_gameObject()->GetComponent<HMUI::ButtonSpriteSwap*>();
            spriteSwap->normalStateSprite = BeatSaberUI::Base64ToSprite(ModSettingsButtonSprite_Normal, 256, 256);
            spriteSwap->disabledStateSprite = spriteSwap->normalStateSprite;
            spriteSwap->highlightStateSprite = BeatSaberUI::Base64ToSprite(ModSettingsButtonSprite_Highlight, 256, 256);
            spriteSwap->pressedStateSprite = spriteSwap->highlightStateSprite;
        }
    }
}

MAKE_HOOK_OFFSETLESS(GameplaySetupViewController_RefreshContent, void, GlobalNamespace::GameplaySetupViewController* self) {
    GameplaySetupViewController_RefreshContent(self);
    self->panels = System::Collections::Generic::List_1<GlobalNamespace::GameplaySetupViewController::Panel*>::New_ctor();
    if (self->showModifiers)
    {
        static auto buttonModifiersStr = il2cpp_utils::createcsstr("BUTTON_MODIFIERS", il2cpp_utils::StringType::Manual);
        self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(Polyglot::Localization::Get(buttonModifiersStr), reinterpret_cast<GlobalNamespace::IRefreshable*>(self->gameplayModifiersPanelController), self->gameplayModifiersPanelController->get_gameObject()));
    }
    else
    {
        self->gameplayModifiersPanelController->get_gameObject()->SetActive(false);
    }
    static auto buttonPlayerSettingsStr = il2cpp_utils::createcsstr("BUTTON_PLAYER_SETTINGS", il2cpp_utils::StringType::Manual);
    self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(Polyglot::Localization::Get(buttonPlayerSettingsStr), reinterpret_cast<GlobalNamespace::IRefreshable*>(self->playerSettingsPanelController), self->playerSettingsPanelController->get_gameObject()));
    if (self->showEnvironmentOverrideSettings)
    {
        static auto buttonEnvironmentsStr = il2cpp_utils::createcsstr("BUTTON_ENVIRONMENTS", il2cpp_utils::StringType::Manual);
        self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(Polyglot::Localization::Get(buttonEnvironmentsStr), reinterpret_cast<GlobalNamespace::IRefreshable*>(self->environmentOverrideSettingsPanelController), self->environmentOverrideSettingsPanelController->get_gameObject()));
    }
    else
    {
        self->environmentOverrideSettingsPanelController->get_gameObject()->SetActive(false);
    }
    if (self->showColorSchemesSettings)
    {
        static auto buttonColorsStr = il2cpp_utils::createcsstr("BUTTON_COLORS", il2cpp_utils::StringType::Manual);
        self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(Polyglot::Localization::Get(buttonColorsStr), reinterpret_cast<GlobalNamespace::IRefreshable*>(self->colorsOverrideSettingsPanelController), self->colorsOverrideSettingsPanelController->get_gameObject()));
    }
    else
    {
        self->colorsOverrideSettingsPanelController->get_gameObject()->SetActive(false);
    }
    
    static auto asd = il2cpp_utils::createcsstr("Test", il2cpp_utils::StringType::Manual);
    UnityEngine::GameObject* testPanel = UnityEngine::GameObject::New_ctor();
    BeatSaberUI::CreateText(testPanel->get_transform(), "Heyyy");
    self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(asd, reinterpret_cast<GlobalNamespace::IRefreshable*>(testPanel->AddComponent(csTypeOf(CustomPanelController*))), testPanel));
    
    System::Collections::Generic::List_1<Il2CppString*>* list = System::Collections::Generic::List_1<Il2CppString*>::New_ctor();
	for(int i = 0; i < self->panels->get_Count(); i++)
	{
        list->Add(self->panels->get_Item(i)->title);
	}
    self->selectionSegmentedControl->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<Il2CppString*>*>(list));
}

void QuestUI::Init() {
    static bool init = false;
    if(!init) {
        init = true;
        il2cpp_functions::Init();
        custom_types::Register::RegisterType<CustomDataType>();
        custom_types::Register::RegisterType<ExternalComponents>();
        custom_types::Register::RegisterType<Backgroundable>();
        custom_types::Register::RegisterType<CustomPanelController>();
        custom_types::Register::RegisterType<ScrollViewContent>();
        custom_types::Register::RegisterType<IncrementSetting>();
        custom_types::Register::RegisterType<ModSettingsButtonsViewController>();
        custom_types::Register::RegisterType<ModSettingsFlowCoordinator>();
        INSTALL_HOOK_OFFSETLESS(getLogger(), OptionsViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "OptionsViewController", "DidActivate", 3));
        INSTALL_HOOK_OFFSETLESS(getLogger(), GameplaySetupViewController_RefreshContent, il2cpp_utils::FindMethodUnsafe("", "GameplaySetupViewController", "RefreshContent", 0));
    }
}

void Register::RegisterModSettings(ModInfo modInfo, bool showModInfo, std::string title, Il2CppReflectionType* il2cpp_type, Register::Type type, Register::DidActivateEvent didActivateEvent) {
    ModSettingsInfos::ModSettingsInfo info = {};
    info.modInfo = modInfo;
    info.showModInfo = showModInfo;
    info.title = title;
    info.type = type;
    info.il2cpp_type = reinterpret_cast<System::Type*>(il2cpp_type);
    info.viewController = nullptr;
    info.flowCoordinator = nullptr;
    info.didActivateEvent = didActivateEvent;
    ModSettingsInfos::add(info);
}