#pragma once

#include "custom-types/shared/macros.hpp"
#include "GameplaySetupMenuTabs.hpp"
#include "UnityEngine/Transform.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, TabHost, Il2CppObject,
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, root);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, OptionallyCreateObject);
    public:
        QuestUI::GameplaySetupMenuTabs::GameplaySetupMenu* menu;
        DECLARE_DEFAULT_CTOR();
)

DECLARE_CLASS_CODEGEN(QuestUI, TabActivator, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(TabHost*, tabHost);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);
)
