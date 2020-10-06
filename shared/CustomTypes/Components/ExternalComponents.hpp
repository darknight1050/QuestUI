#pragma once
#include <vector>
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Component.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ExternalComponents, UnityEngine::MonoBehaviour,

    private:
        std::vector<UnityEngine::Component*> components;
    public:
        template<class T = UnityEngine::Component*>
        T Get() {
            return (T)GetByType(typeof(T));
        }

    DECLARE_METHOD(void, Add, UnityEngine::Component* component); 
    DECLARE_METHOD(UnityEngine::Component*, GetByType, Il2CppReflectionType* type); 
    
    REGISTER_FUNCTION(ExternalComponents,
        REGISTER_METHOD(GetByType);
    )
)