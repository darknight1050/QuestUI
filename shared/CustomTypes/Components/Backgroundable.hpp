#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Image.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, Backgroundable, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, background);

    DECLARE_METHOD(void, ApplyBackgroundWithAlpha, Il2CppString* name, float alpha);

    DECLARE_METHOD(void, ApplyBackground, Il2CppString* name);
    
    REGISTER_FUNCTION(Backgroundable,
        REGISTER_FIELD(background);
        
        REGISTER_METHOD(ApplyBackgroundWithAlpha);
        REGISTER_METHOD(ApplyBackground);
    )
    
)