#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "QuestUI.hpp"
#include "GameplaySetupMenuTabs.hpp"
#include "CustomTypes/Components/SegmentedControl/CustomTextSegmentedControlData.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, GameplaySetup, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(QuestUI::CustomTextSegmentedControlData*, segmentedController);
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
        static constexpr int tabCount = 2;
)