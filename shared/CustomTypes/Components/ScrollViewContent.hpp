#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ScrollView.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ScrollViewContent, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(HMUI::ScrollView*, scrollView);

    DECLARE_METHOD(void, Start);
    DECLARE_METHOD(void, OnEnable);
    DECLARE_METHOD(void, OnRectTransformDimensionsChange);
    DECLARE_METHOD(void, UpdateScrollView); 
    
    REGISTER_FUNCTION(ScrollViewContent,
        REGISTER_FIELD(scrollView);
        
        REGISTER_METHOD(Start);
        REGISTER_METHOD(OnEnable);
        REGISTER_METHOD(OnRectTransformDimensionsChange);
        REGISTER_METHOD(UpdateScrollView);
    )
)