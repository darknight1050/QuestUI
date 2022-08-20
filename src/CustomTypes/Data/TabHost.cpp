#include "CustomTypes/Data/TabHost.hpp"
#include "customlogger.hpp"

#include "UnityEngine/GameObject.hpp"

DEFINE_TYPE(QuestUI, TabHost);
DEFINE_TYPE(QuestUI, TabActivator);

namespace QuestUI {
    void TabHost::PostParse() {
        getLogger().info("TabHost PostParse for menu %s", menu->title.c_str());
        if (!root || !root->m_CachedPtr.m_value) {
            return;
        }
        auto activator = root->get_gameObject()->AddComponent<TabActivator*>();
        activator->tabHost = this;

        auto c = menu->GetComponent();
        if (!c && menu->il2cpp_type) {
            menu->CreateObject(root);
        }
    }

    void TabActivator::OnEnable() {
        tabHost->menu->Activate();
    }

    void TabActivator::OnDisable() {
        tabHost->menu->Deactivate();
    }
}