#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/register.hpp"

#include "QuestUI.hpp"
#include "ModSettingsInfos.hpp"
#include "CustomTypes/Data/CustomDataType.hpp"
#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "CustomTypes/Components/CustomPanelController.hpp"
#include "CustomTypes/Components/ScrollViewContent.hpp"
#include "CustomTypes/Components/MainThreadScheduler.hpp"
#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "CustomTypes/Components/FlowCoordinators/ModSettingsFlowCoordinator.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenManager.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenMoverPointer.hpp"

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
#include "GlobalNamespace/UIKeyboardManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "Polyglot/Localization.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "System/Collections/Generic/IReadOnlyList_1.hpp"

#include "HMUI/TextPageScrollView.hpp"

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

MAKE_HOOK_MATCH(OptionsViewController_DidActivate, &GlobalNamespace::OptionsViewController::DidActivate, void, GlobalNamespace::OptionsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    OptionsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if(firstActivation) {
        flowCoordinator = nullptr;
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

MAKE_HOOK_MATCH(GameplaySetupViewController_RefreshContent, &GlobalNamespace::GameplaySetupViewController::RefreshContent, void, GlobalNamespace::GameplaySetupViewController* self) {
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
    static auto buttonPlayerSettingsStr = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BUTTON_PLAYER_OPTIONS");
    self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(Polyglot::Localization::Get(buttonPlayerSettingsStr), reinterpret_cast<GlobalNamespace::IRefreshable*>(self->playerSettingsPanelController), self->playerSettingsPanelController->get_gameObject()));
    if (self->showEnvironmentOverrideSettings)
    {
        static auto buttonEnvironmentsStr = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BUTTON_ENVIRONMENTS");
        self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(Polyglot::Localization::Get(buttonEnvironmentsStr), reinterpret_cast<GlobalNamespace::IRefreshable*>(self->environmentOverrideSettingsPanelController), self->environmentOverrideSettingsPanelController->get_gameObject()));
    }
    else
    {
        self->environmentOverrideSettingsPanelController->get_gameObject()->SetActive(false);
    }
    if (self->showColorSchemesSettings)
    {
        static auto buttonColorsStr = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BUTTON_COLORS");
        self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(Polyglot::Localization::Get(buttonColorsStr), reinterpret_cast<GlobalNamespace::IRefreshable*>(self->colorsOverrideSettingsPanelController), self->colorsOverrideSettingsPanelController->get_gameObject()));
    }
    else
    {
        self->colorsOverrideSettingsPanelController->get_gameObject()->SetActive(false);
    }
    
    static auto asd = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Test");
    UnityEngine::GameObject* testPanel = BeatSaberUI::CreateScrollView(self->get_transform());
    testPanel->AddComponent<UnityEngine::UI::LayoutElement*>();
    //testPanel->get_transform()->SetParent(self->get_transform());
    //UnityEngine::UI::GridLayoutGroup* layout = BeatSaberUI::CreateGridLayoutGroup(testPanel->get_transform());
    //UnityEngine::UI::ContentSizeFitter* contentSizeFitter = testPanel->AddComponent<UnityEngine::UI::ContentSizeFitter*>();
    //contentSizeFitter->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
    //contentSizeFitter->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);

    BeatSaberUI::CreateText(testPanel->get_transform(), "Heyyy")->get_transform()->GetParent()->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(80.0f);
    BeatSaberUI::CreateToggle(testPanel->get_transform(), "TestToggle", false,
        [](bool value) {
            getLogger().debug("ToggleWasPressed");
        })->get_transform()->GetParent()->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(80.0f);
    self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(asd, reinterpret_cast<GlobalNamespace::IRefreshable*>(testPanel->AddComponent(csTypeOf(CustomPanelController*))), testPanel));

    testPanel->SetActive(false);

    //static auto asd1 = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Test1");
    //UnityEngine::GameObject* testPanel1 = BeatSaberUI::CreateScrollView(self->get_transform());
    //testPanel1->AddComponent<UnityEngine::UI::LayoutElement*>();

    //BeatSaberUI::CreateText(testPanel1->get_transform(), "Heyyy1")->get_transform()->GetParent()->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(80.0f);
    //BeatSaberUI::CreateToggle(testPanel1->get_transform(), "TestToggle1", false,
    //    [](bool value) {
    //        getLogger().debug("Toggle1WasPressed");
    //    })->get_transform()->GetParent()->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(80.0f);
    //self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(asd1, reinterpret_cast<GlobalNamespace::IRefreshable*>(testPanel1->AddComponent(csTypeOf(CustomPanelController*))), testPanel1));

    //testPanel1->SetActive(false);

    //static auto asd2 = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Test2");
    //UnityEngine::GameObject* testPanel2 = BeatSaberUI::CreateScrollView(self->get_transform());
    //testPanel2->AddComponent<UnityEngine::UI::LayoutElement*>();

    //BeatSaberUI::CreateText(testPanel2->get_transform(), "Heyyy2")->get_transform()->GetParent()->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(80.0f);
    //BeatSaberUI::CreateToggle(testPanel2->get_transform(), "TestToggle2", false,
    //    [](bool value) {
    //        getLogger().debug("Toggle2WasPressed");
    //    })->get_transform()->GetParent()->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(80.0f);
    //self->panels->Add(GlobalNamespace::GameplaySetupViewController::Panel::New_ctor(asd2, reinterpret_cast<GlobalNamespace::IRefreshable*>(testPanel2->AddComponent(csTypeOf(CustomPanelController*))), testPanel2));

    //testPanel2->SetActive(false);

    System::Collections::Generic::List_1<Il2CppString*>* list = System::Collections::Generic::List_1<Il2CppString*>::New_ctor();
	for(int i = 0; i < self->panels->get_Count(); i++)
	{
        list->Add(self->panels->get_Item(i)->title);
	}
    self->selectionSegmentedControl->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<Il2CppString*>*>(list));

    //BeatSaberUI::CreateScrollView(selectionSegmentedControl->get_transform());

    //self->selectionSegmentedControl->get_transform()->GetParent()->get_gameObject()->AddComponent<HMUI::TextPageScrollView*>();
}

MAKE_HOOK_MATCH(SceneManager_Internal_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene prevScene, UnityEngine::SceneManagement::Scene nextScene) {
    SceneManager_Internal_ActiveSceneChanged(prevScene, nextScene);
    BeatSaberUI::ClearCache();
    if(prevScene.IsValid() && nextScene.IsValid()) {
        std::string prevSceneName = to_utf8(csstrtostr(prevScene.get_name()));
        std::string nextSceneName = to_utf8(csstrtostr(nextScene.get_name()));
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
            std::string prevSceneName = to_utf8(csstrtostr(prevScene.get_name()));
            getLogger().info("Scene change from %s to null", prevSceneName.c_str());
        } 
        if(nextScene.IsValid()) {
            std::string nextSceneName = to_utf8(csstrtostr(nextScene.get_name()));
            getLogger().info("Scene change from null to %s", nextSceneName.c_str());
        }
    }
}

MAKE_HOOK_MATCH(UIKeyboardManager_OpenKeyboardFor, &GlobalNamespace::UIKeyboardManager::OpenKeyboardFor, void, GlobalNamespace::UIKeyboardManager* self, HMUI::InputFieldView* input) {
    static UnityEngine::Vector3 magicVector = UnityEngine::Vector3(1337.0f, 1337.0f, 1337.0f);
    if (input->keyboardPositionOffset == magicVector) {
        auto transform = input->get_transform();
        if(transform->get_position().y < 1.278000f) {
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
        INSTALL_HOOK(getLogger(), GameplaySetupViewController_RefreshContent);
        getLogger().info("Init completed!");
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