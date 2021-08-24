#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/register.hpp"

#include "QuestUI.hpp"
#include "ModSettingsInfos.hpp"
#include "CustomTypes/Components/ExternalComponents.hpp"
#include "CustomTypes/Data/CustomDataType.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"
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
#include "GlobalNamespace/UIKeyboardManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"

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
        /*
        Register::RegisterModSettingsViewController({"questui", VERSION}, "testList", [](HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
            auto container = BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());
            BeatSaberUI::CreateText(container->get_transform(), "Cool list header text or something idk");
            auto listWrapper = new LambdaCellListWrapper([](LambdaCellListWrapper*){ getLogger().info("GetTableSize"); return 100; }, [](LambdaCellListWrapper*, QuestUI::CustomCellTableCell* cell, int idx){
                getLogger().info("Setup cell %d", idx);
                cell->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 0.5f);
                auto vertical = BeatSaberUI::CreateVerticalLayoutGroup(cell->get_transform());
                auto horizontal = BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());
                horizontal->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 0.8f);

                cell->CustomCellTableCell::selectedGroup->Add(BeatSaberUI::CreateText(horizontal->get_transform(), string_format("selected-%d", idx))->get_gameObject());
                cell->CustomCellTableCell::hoveredGroup->Add(BeatSaberUI::CreateText(horizontal->get_transform(), string_format("highlighted-%d", idx))->get_gameObject());
                cell->CustomCellTableCell::neitherGroup->Add(BeatSaberUI::CreateText(horizontal->get_transform(), string_format("neither-%d", idx))->get_gameObject());
            }, [](LambdaCellListWrapper*, QuestUI::CustomCellTableCell* cell, int idx){
                getLogger().info("cell %d selected!", idx);
            }, [](LambdaCellListWrapper*){});

            auto tableData = BeatSaberUI::CreateScrollableCustomCellList(container->get_transform(), {80.0f, 50.0f}, listWrapper);
            tableData->tableView->ReloadData();
        });
        */

        Register::RegisterModSettingsViewController({"questui", VERSION}, "testList", [](HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
            if (!firstActivation) return;
            auto container = BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());
            BeatSaberUI::CreateText(container->get_transform(), "Cool list header text or something idk");

            auto tableData = BeatSaberUI::CreateScrollableList(container->get_transform(), {50.0f, 50.0f}, [](int idx){
                getLogger().info("idx %d selected!", idx);
            });

            tableData->data.emplace_back("dicks", "bullshit", nullptr);
            tableData->data.emplace_back("pink cute", "no denyin", nullptr);
            tableData->data.emplace_back("this shit rocks", "if it works at all", nullptr);
            tableData->data.emplace_back("whatcha got there", "", nullptr);
            tableData->data.emplace_back("a", "smoothie", nullptr);
            tableData->data.emplace_back("", "just sub", nullptr);

            for (int i = 0; i < 100; i++)
            {
                tableData->data.emplace_back(string_format("text-%d", i), string_format("%d-subText", i));
            }
            
            tableData->tableView->ReloadData();
            BeatSaberUI::CreateText(container->get_transform(), "this is the bottom of the VC");

            BeatSaberUI::CreateSliderSetting(container->get_transform(), "dick size (in)", 0.05f, 69.0f, 0.0f, 100.0f, 1.0f, [](float val){
                getLogger().info("My dick is %.2f inches long", val);
            });

        });
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