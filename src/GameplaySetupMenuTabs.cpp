#include "GameplaySetupMenuTabs.hpp"
#include "CustomTypes/Components/GameplaySetup.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/GameplaySetup/GameplaySetupMenu.hpp"
#include "CustomTypes/Data/TabHost.hpp"

namespace QuestUI::GameplaySetupMenuTabs {
    std::vector<GameplaySetupMenu*> all = {};
    std::vector<GameplaySetupMenu*> solo = {};
    std::vector<GameplaySetupMenu*> online = {};
    std::vector<GameplaySetupMenu*> campaign = {};
    std::vector<GameplaySetupMenu*> custom = {};

    std::vector<GameplaySetupMenu*>& get(Register::MenuType type)
    {
        switch(type)
        {
            case Register::MenuType::Solo:
                return solo;
            case Register::MenuType::Online:
                return online;
            case Register::MenuType::Campaign:
                return campaign;
            case Register::MenuType::Custom:
                return custom;
            default:
                return all;
        }
    }
    
    void add(GameplaySetupMenu* menu)
    {
        CRASH_UNLESS(menu);
        all.push_back(menu);
        if (menu->type & Register::MenuType::Solo) solo.push_back(menu);
        if (menu->type & Register::MenuType::Online) online.push_back(menu);
        if (menu->type & Register::MenuType::Campaign) campaign.push_back(menu);
        if (menu->type & Register::MenuType::Custom) custom.push_back(menu);

        BSML::MenuType t = static_cast<BSML::MenuType>(static_cast<int>(menu->type));
    	auto host = TabHost::New_ctor();
        host->menu = menu;

        BSML::Register::RegisterGameplaySetupTab(menu->title, MOD_ID "_gameplay", host, t);
    }

    void GameplaySetupMenu::CreateObject(UnityEngine::Transform* parent)
    {
        // if already made, return the made component
        static ConstString menuName("QuestUIGameplaySetupMenu");
        gameObject = UnityEngine::GameObject::New_ctor(menuName);
        gameObject->get_transform()->SetParent(parent, false);
        if (il2cpp_type) // component type
        {
            gameObject->AddComponent(il2cpp_type);
        }
        gameObject->AddComponent<GameplaySetupTabMB*>()->Init(this);
    }

    UnityEngine::Component* GameplaySetupMenu::GetComponent()
    {
        if (!il2cpp_type) return nullptr;
        if (!gameObject || !gameObject->m_CachedPtr.m_value) return nullptr;
        return gameObject->GetComponent(il2cpp_type);
    }

    void GameplaySetupMenu::Activate()
    {
        if (il2cpp_type) // component type
        {
            auto component = GetComponent();
            auto* didActivate = il2cpp_functions::class_get_method_from_name(il2cpp_utils::ExtractClass (component), "DidActivate", 1);
            if (didActivate)
                il2cpp_utils::RunMethod(component, didActivate, !activatedBefore);
            activatedBefore = true;
        }
        else // callback type
        {
            gameplaySetupMenuEvent(gameObject, !activatedBefore);
            activatedBefore = true;
        }

        gameObject->SetActive(true);
    }

    void GameplaySetupMenu::Deactivate()
    {
        if (il2cpp_type) // component type
        {
            auto component = GetComponent();
            auto* didDeactivate = il2cpp_functions::class_get_method_from_name(il2cpp_utils::ExtractClass (component), "DidDeactivate", 0);
            if (didDeactivate)
                il2cpp_utils::RunMethod(component, didDeactivate, !activatedBefore);
        }

        gameObject->SetActive(false);
    }
}