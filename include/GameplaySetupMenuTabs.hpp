#pragma once

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "QuestUI.hpp"

namespace QuestUI::GameplaySetupMenuTabs {
    struct GameplaySetupMenu {
        ModInfo modInfo;
        std::string title;
        Register::MenuType type;
        System::Type* il2cpp_type;
        UnityEngine::GameObject* gameObject;
        Register::GameplaySetupMenuEvent gameplaySetupMenuEvent;
        
        bool activatedBefore = false;
        void CreateObject(UnityEngine::Transform* parent);
        UnityEngine::Component* GetComponent();
        bool IsMenuType(Register::MenuType type) { return type & this->type; };

        void Activate();
        void Deactivate();
    };

    std::vector<GameplaySetupMenu*>& get(Register::MenuType type = Register::MenuType::All);
    void add(GameplaySetupMenu* menu);
}