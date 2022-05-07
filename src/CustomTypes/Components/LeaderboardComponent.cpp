
#include "CustomTypes/Components/LeaderboardComponent.hpp"

DEFINE_TYPE(QuestUI, LeaderboardComponent)

void QuestUI::LeaderboardComponent::Add(UnityEngine::Component* component) {
    components.push_back(component);
}

UnityEngine::Component* QuestUI::LeaderboardComponent::GetByType(Il2CppReflectionType *type) {
    if (!type) return nullptr;
    Il2CppClass* clazz = il2cpp_functions::class_from_system_type(type);
    for(UnityEngine::Component* component : components)
    {
        if (il2cpp_functions::class_is_assignable_from(clazz, il2cpp_functions::object_get_class(component)))
            return component;
    }
    return nullptr;
}