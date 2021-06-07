#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ScrollView.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ScrollViewContent, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(HMUI::ScrollView*, scrollView);
    DECLARE_INSTANCE_FIELD(bool, inSetup);

    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, OnRectTransformDimensionsChange);
    DECLARE_INSTANCE_METHOD(void, UpdateScrollView); 
)
