#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/register.hpp"

#include "QuestUI.hpp"
#include "ModSettingsInfos.hpp"
#include "MainMenuModSettingInfos.hpp"
#include "GameplaySetupMenuTabs.hpp"
#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Data/CustomDataType.hpp"
#include "CustomTypes/Components/GameplaySetup.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Components/ScrollViewContent.hpp"
#include "CustomTypes/Components/MainThreadScheduler.hpp"
#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "CustomTypes/Components/FlowCoordinators/ModSettingsFlowCoordinator.hpp"
#include "CustomTypes/Components/ViewControllers/MainMenuModSettingsViewController.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenManager.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenMoverPointer.hpp"

#include "BeatSaberUI.hpp"
#include "InternalBeatSaberUI.hpp"
#include "Sprites/ModSettingsButton.hpp"

#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/UIKeyboardManager.hpp"

#include "GlobalNamespace/PlayerSettingsPanelController.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/MultiplayerSettingsPanelController.hpp"
#include "GlobalNamespace/ColorsOverrideSettingsPanelController.hpp"
#include "GlobalNamespace/EnvironmentOverrideSettingsPanelController.hpp"

#include "GlobalNamespace/PlayerOptionsViewController.hpp"
#include "GlobalNamespace/PlayerStatisticsViewController.hpp"


#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"
#include "HMUI/TextSegmentedControl.hpp"

#include "Polyglot/Localization.hpp"

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

MAKE_HOOK_MATCH(GameplaySetupViewController_DidActivate, &GlobalNamespace::GameplaySetupViewController::DidActivate, void, GlobalNamespace::GameplaySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    GameplaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    
    auto gameplaySetup = self->GetComponent<QuestUI::GameplaySetup*>();
    if (!gameplaySetup) gameplaySetup = self->get_gameObject()->AddComponent<QuestUI::GameplaySetup*>();
    gameplaySetup->Activate(firstActivation);
}

MAKE_HOOK_MATCH(OptionsViewController_DidActivate, &GlobalNamespace::OptionsViewController::DidActivate, void, GlobalNamespace::OptionsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    OptionsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if(firstActivation) {
        flowCoordinator = nullptr;
        if(GetModsCount() > 0) {
            UnityEngine::UI::Button* avatarButton = self->settingsButton;
            UnityEngine::UI::Button* button = UnityEngine::Object::Instantiate(avatarButton);
            static auto modSettingsStr = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Mod Settings");
            button->set_name(modSettingsStr);
            static auto wrapperStr = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Wrapper");
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

MAKE_HOOK_MATCH(MainFlowCoordinator_DidActivate, &GlobalNamespace::MainFlowCoordinator::DidActivate, void, GlobalNamespace::MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    getLogger().info("MainFlowCoordinator_DidActivate called!");
	// when activating, we want to provide our own view controller for the left screen, so just take whatever is activated and display ours for right
    
    if (firstActivation && MainMenuModSettingInfos::get().size() > 0)
    {
        auto vc = BeatSaberUI::CreateViewController<MainMenuModSettingsViewController*>();
        self->providedLeftScreenViewController = vc;
    	MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
        self->providedLeftScreenViewController = vc;
        self->SetLeftScreenViewController(vc, HMUI::ViewController::AnimationType::In);
    }
    else MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_MATCH(MainFlowCoordinator_TopViewControllerWillChange, &GlobalNamespace::MainFlowCoordinator::TopViewControllerWillChange, void, GlobalNamespace::MainFlowCoordinator* self, HMUI::ViewController* oldViewController, HMUI::ViewController* newViewController, HMUI::ViewController::AnimationType animationType)
{
    // doesnt call orig!
    if (newViewController->Equals(self->mainMenuViewController))
	{
		self->SetLeftScreenViewController(self->providedLeftScreenViewController, animationType);
		self->SetRightScreenViewController(self->providedRightScreenViewController, animationType);
		self->SetBottomScreenViewController(nullptr, animationType);
	}
	else
	{
		self->SetLeftScreenViewController(nullptr, animationType);
		self->SetRightScreenViewController(nullptr, animationType);
		self->SetBottomScreenViewController(nullptr, animationType);
	}

	if (newViewController->Equals(self->howToPlayViewController))
	{
        static auto LABEL_HOW_TO_PLAY = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("LABEL_HOW_TO_PLAY");
        self->SetTitle(Polyglot::Localization::Get(LABEL_HOW_TO_PLAY), animationType);
		self->SetBottomScreenViewController(self->playerStatisticsViewController, animationType);
        self->set_showBackButton(true);
		return;
	}
	if (newViewController->Equals(self->playerOptionsViewController))
	{
        static auto BUTTON_PLAYER_OPTIONS = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BUTTON_PLAYER_OPTIONS");
        self->SetTitle(Polyglot::Localization::Get(BUTTON_PLAYER_OPTIONS), animationType);
        self->set_showBackButton(true);
		return;
	}
	if (newViewController->Equals(self->optionsViewController))
	{
        static auto LABEL_OPTIONS = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("LABEL_OPTIONS");
        self->SetTitle(Polyglot::Localization::Get(LABEL_OPTIONS), animationType);
        self->set_showBackButton(true);
		return;
	}
	self->SetTitle(il2cpp_utils::newcsstr(""), animationType);
    self->set_showBackButton(false);
}

MAKE_HOOK_MATCH(SceneManager_Internal_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene prevScene, UnityEngine::SceneManagement::Scene nextScene) {
    SceneManager_Internal_ActiveSceneChanged(prevScene, nextScene);
    BeatSaberUI::ClearCache();
    if(prevScene.IsValid() && nextScene.IsValid()) {
        std::string prevSceneName(to_utf8(csstrtostr(prevScene.get_name())));
        std::string nextSceneName(to_utf8(csstrtostr(nextScene.get_name())));
        getLogger().info("Scene change from %s to %s", prevSceneName.c_str(), nextSceneName.c_str());
        static bool hasInited = false;
        if(prevSceneName == "QuestInit"){
            hasInited = true;
        }
        if(hasInited && prevSceneName == "EmptyTransition" && nextSceneName.find("Menu") != std::string::npos) {
            hasInited = false;
            BeatSaberUI::SetupPersistentObjects();
        }
    } else {
        if(prevScene.IsValid()) {
            std::string prevSceneName(to_utf8(csstrtostr(prevScene.get_name())));
            getLogger().info("Scene change from %s to null", prevSceneName.c_str());
        } 
        if(nextScene.IsValid()) {
            std::string nextSceneName(to_utf8(csstrtostr(nextScene.get_name())));
            getLogger().info("Scene change from null to %s", nextSceneName.c_str());
        }
    }
}

MAKE_HOOK_MATCH(UIKeyboardManager_OpenKeyboardFor, &GlobalNamespace::UIKeyboardManager::OpenKeyboardFor, void, GlobalNamespace::UIKeyboardManager* self, HMUI::InputFieldView* input) {
    static UnityEngine::Vector3 magicVector = UnityEngine::Vector3(1337.0f, 1337.0f, 1337.0f);
    if (input->keyboardPositionOffset == magicVector) {
        auto transform = input->get_transform();
        if(transform->get_position().y < 1.72f) {
            input->keyboardPositionOffset = UnityEngine::Vector3(0.0f, 42.0f, 0.0f);
        } else {
            input->keyboardPositionOffset = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
        }
        UIKeyboardManager_OpenKeyboardFor(self, input);
        input->keyboardPositionOffset = magicVector;
    } else {
        UIKeyboardManager_OpenKeyboardFor(self, input);
    }
}

bool didInit = false;

bool DidInit(){
    return didInit;
}

void QuestUI::Init() {
    if(!didInit) {
        didInit = true;
        getLogger().info("Init started...");
        il2cpp_functions::Init();
        custom_types::Register::AutoRegister();
        INSTALL_HOOK(getLogger(), OptionsViewController_DidActivate);
        INSTALL_HOOK(getLogger(), SceneManager_Internal_ActiveSceneChanged);
        INSTALL_HOOK(getLogger(), UIKeyboardManager_OpenKeyboardFor);
        INSTALL_HOOK(getLogger(), GameplaySetupViewController_DidActivate);
        INSTALL_HOOK(getLogger(), MainFlowCoordinator_DidActivate);
        INSTALL_HOOK_ORIG(getLogger(), MainFlowCoordinator_TopViewControllerWillChange);
    }
}

void Register::RegisterModSettings(ModInfo modInfo, bool showModInfo, std::string title, Il2CppReflectionType* il2cpp_type, Register::Type type, Register::DidActivateEvent didActivateEvent) {
    Init();
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

void Register::RegisterGameplaySetupMenu(ModInfo modInfo, std::string_view title, Il2CppReflectionType* il2cpp_type, int type, GameplaySetupMenuEvent setupEvent) {
    Init();
    GameplaySetupMenuTabs::GameplaySetupMenu* menu = new GameplaySetupMenuTabs::GameplaySetupMenu();
    menu->modInfo = modInfo;
    menu->title = title;
    menu->type = type;
    menu->il2cpp_type = reinterpret_cast<System::Type*>(il2cpp_type);
    menu->gameObject = nullptr;
    menu->gameplaySetupMenuEvent = setupEvent;
    GameplaySetupMenuTabs::add(menu);
}

void Register::RegisterMainMenuModSettings(ModInfo modInfo, bool showModInfo, std::string title, Il2CppReflectionType* il2cpp_type, Register::Type type, Register::DidActivateEvent didActivateEvent) {
    Init();
    ModSettingsInfos::ModSettingsInfo info = {};
    info.modInfo = modInfo;
    info.showModInfo = showModInfo;
    info.title = title;
    info.type = type;
    info.il2cpp_type = reinterpret_cast<System::Type*>(il2cpp_type);
    info.viewController = nullptr;
    info.flowCoordinator = nullptr;
    info.didActivateEvent = didActivateEvent;
    MainMenuModSettingInfos::add(info);
}