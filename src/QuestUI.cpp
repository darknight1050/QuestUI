#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/delegate.hpp"

#include "QuestUI.hpp"
#include "ModSettingsInfos.hpp"
#include "GameplaySetupMenuTabs.hpp"

#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Data/CustomDataType.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Components/ScrollViewContent.hpp"
#include "CustomTypes/Components/MainThreadScheduler.hpp"
#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenManager.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenMoverPointer.hpp"

#include "BeatSaberUI.hpp"
#include "InternalBeatSaberUI.hpp"
#include "Sprites/ModSettingsButton.hpp"

#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/UIKeyboardManager.hpp"
#include "GlobalNamespace/HelpFlowCoordinator.hpp"

#include "GlobalNamespace/PlayerSettingsPanelController.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/MultiplayerSettingsPanelController.hpp"
#include "GlobalNamespace/ColorsOverrideSettingsPanelController.hpp"
#include "GlobalNamespace/EnvironmentOverrideSettingsPanelController.hpp"

#include "GlobalNamespace/PlayerOptionsViewController.hpp"
#include "GlobalNamespace/PlayerStatisticsViewController.hpp"

#include "GlobalNamespace/RichPresenceManager.hpp"

#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "System/Action_1.hpp"
#include "Zenject/DiContainer.hpp"

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
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/ModalView.hpp"

#include "Polyglot/Localization.hpp"

#include "customlogger.hpp"

#include "bsml/shared/BSMLDataCache.hpp"
#include "assets.hpp"

using namespace QuestUI;

Logger& getLogger() {
    static auto logger = new Logger(modloader::ModInfo{"questui", VERSION, GIT_COMMIT}, LoggerOptions(false, true));
    return *logger;
}

int QuestUI::GetModsCount() {
    return ModSettingsInfos::get().size();
}

static bool hasInited = false;
static bool shouldClear = false;

// do things with the scene transition stuff
MAKE_HOOK_MATCH(RichPresenceManager_HandleGameScenesManagerTransitionDidFinish, &GlobalNamespace::RichPresenceManager::HandleGameScenesManagerTransitionDidFinish, void, GlobalNamespace::RichPresenceManager* self, GlobalNamespace::ScenesTransitionSetupDataSO* setupData, Zenject::DiContainer* container) {
    RichPresenceManager_HandleGameScenesManagerTransitionDidFinish(self, setupData, container);

    if (shouldClear) {
        shouldClear = false;
        BeatSaberUI::ClearCache();
        if (hasInited) {
            hasInited = false;
            BeatSaberUI::SetupPersistentObjects();
        }
    }
}

// Here we just check if we should be doing things after all the scene transitions are done:
MAKE_HOOK_MATCH(SceneManager_Internal_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene prevScene, UnityEngine::SceneManagement::Scene nextScene) {
    SceneManager_Internal_ActiveSceneChanged(prevScene, nextScene);
    bool prevValid = prevScene.IsValid(), nextValid = nextScene.IsValid();

    if (prevValid && nextValid) {
        std::string prevSceneName(prevScene.get_name());
        std::string nextSceneName(nextScene.get_name());
        getLogger().info("Scene change from %s to %s", prevSceneName.c_str(), nextSceneName.c_str());

        if (prevSceneName == "QuestInit") hasInited = true;

        // if we just inited, and aren't already going to clear, check the next scene name for the menu
        if (hasInited && !shouldClear && nextSceneName.find("Menu") != std::u16string::npos) {
            getLogger().info("Queueing up a clear");
            shouldClear = true;
        }
    } else if (prevValid) {
        std::string prevSceneName(prevScene.get_name());
        getLogger().info("Scene change from %s to null", prevSceneName.c_str());
    } else if (nextValid) {
        std::string nextSceneName(nextScene.get_name());
        getLogger().info("Scene change from null to %s", nextSceneName.c_str());
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

MAKE_HOOK_MATCH(MenuTransitionsHelper_RestartGame, &GlobalNamespace::MenuTransitionsHelper::RestartGame, void, GlobalNamespace::MenuTransitionsHelper* self, System::Action_1<Zenject::DiContainer*>* finishCallback)
{
    getLogger().info("Restart Game hook");
    for (auto& info : ModSettingsInfos::get()) {
        if (info.viewController && info.viewController->m_CachedPtr.m_value) {
            // we destroy the attached GO first, and we cast to element to prevent logs from bs hook trying to find the method on the wrong type,
            // since they are at the same offset
            UnityEngine::Object::DestroyImmediate(((UnityEngine::Component*)info.viewController)->get_gameObject());
        }
        // since viewcontroller is at the same offset as flowcoordinator we do not need to check what it is
        info.viewController = nullptr;
    }

    // we should also clear the tabs
    for (auto& tab : GameplaySetupMenuTabs::get()) {
        if (tab->gameObject && tab->gameObject->m_CachedPtr.m_value) {
            UnityEngine::Object::DestroyImmediate(tab->gameObject);
        }
        tab->gameObject = nullptr;
        tab->activatedBefore = false;
    }

    // everything has been destroyed, clear cache!
    MenuTransitionsHelper_RestartGame(self, finishCallback);
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
        // needed atm for ClickableImage
        il2cpp_functions::Class_Init(classof(HMUI::ImageView*));
        // needed atm for ClickableText
        il2cpp_functions::Class_Init(classof(HMUI::CurvedTextMeshPro*));

        custom_types::Register::AutoRegister();
        INSTALL_HOOK(getLogger(), SceneManager_Internal_ActiveSceneChanged);
        INSTALL_HOOK(getLogger(), UIKeyboardManager_OpenKeyboardFor);
        INSTALL_HOOK(getLogger(), RichPresenceManager_HandleGameScenesManagerTransitionDidFinish);
        INSTALL_HOOK(getLogger(), MenuTransitionsHelper_RestartGame);

    }
}

void Register::RegisterModSettings(modloader::ModInfo modInfo, bool showModInfo, std::string title, Register::MenuLocation location, Il2CppReflectionType* il2cpp_type, Register::Type type, Register::DidActivateEvent didActivateEvent) {
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
    info.location = location;
    ModSettingsInfos::add(info);
}


void Register::RegisterGameplaySetupMenu(modloader::ModInfo modInfo, std::string_view title, Il2CppReflectionType* il2cpp_type, int type, GameplaySetupMenuEvent setupEvent) {
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


BSML_DATACACHE(settings) {
    return IncludedAssets::SettingsHost_bsml;
}

BSML_DATACACHE(gameplay) {
    return IncludedAssets::TabHost_bsml;
}