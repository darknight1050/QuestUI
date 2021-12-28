#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "QuestUI.hpp"
#include "GameplaySetupMenuTabs.hpp"
#include "CustomTypes/Components/SegmentedControl/CustomTextSegmentedControlData.hpp"
#include "UnityEngine/UI/Button.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, GameplaySetup, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(QuestUI::CustomTextSegmentedControlData*, segmentedController);
    DECLARE_INSTANCE_FIELD(QuestUI::CustomTextSegmentedControlData*, moddedController);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, leftButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, rightButton);

    DECLARE_INSTANCE_METHOD(void, OnDisable);
    DECLARE_CTOR(ctor);
    
    public:
        void Setup();
        void Activate(bool firstActivation);
        Register::MenuType GetMenuType();

        std::vector<GameplaySetupMenuTabs::GameplaySetupMenu*> currentTabs;
        int currentFirst = 0;
        GameplaySetupMenuTabs::GameplaySetupMenu* currentMenu = nullptr;

        void SwitchGameplayTab(int idx);
        void ChooseModSegment(int idx);
        void SetModTexts();
        void MoveModMenus(int offset);
        // gets the indexes of the slice of current tabs to display
        std::vector<int> get_slice();
        static constexpr int tabCount = 2;
)


// Custom type to mark game object as yeeted, WeakPtr magic
DECLARE_CLASS_CODEGEN(QuestUI, GameplaySetupTabMB, UnityEngine::MonoBehaviour,
private:
    DECLARE_INSTANCE_METHOD(void, OnDestroy);
    GameplaySetupMenuTabs::GameplaySetupMenu* assignedMenu;
public:
        void Init(GameplaySetupMenuTabs::GameplaySetupMenu* assignedMenu);
)