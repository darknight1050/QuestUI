#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ScrollView.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ScrollViewContent, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(HMUI::ScrollView*, scrollView);
    DECLARE_INSTANCE_FIELD(bool, inSetup);

    DECLARE_METHOD(void, Start);
    DECLARE_METHOD(void, OnEnable);
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, OnRectTransformDimensionsChange);
    DECLARE_METHOD(void, UpdateScrollView); 
    
    REGISTER_FUNCTION(ScrollViewContent,
        REGISTER_FIELD(scrollView);
        REGISTER_FIELD(inSetup);
        
        REGISTER_METHOD(Start);
        REGISTER_METHOD(OnEnable);
        REGISTER_METHOD(Update);
        REGISTER_METHOD(OnRectTransformDimensionsChange);
        REGISTER_METHOD(UpdateScrollView);
    )
)