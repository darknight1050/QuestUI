#pragma once
#include <vector>
#include "UnityEngine/MonoBehaviour.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, MainThreadScheduler, UnityEngine::MonoBehaviour,

    private:
        static std::vector<std::function<void()>> scheduledMethods;
        static std::mutex scheduledMethodsMutex;
    public:
        static void Schedule(std::function<void()> method) {
            using CurrentThreadIsMainThreadMethod = function_ptr_t<bool>;
            static CurrentThreadIsMainThreadMethod currentThreadIsMainThread = il2cpp_utils::resolve_icall<bool>("UnityEngine.Object::CurrentThreadIsMainThread");
            if(currentThreadIsMainThread()) {
                method();
                return;
            }
            std::lock_guard<std::mutex> lock(scheduledMethodsMutex);
            scheduledMethods.push_back(method);
        }
    
    DECLARE_INSTANCE_METHOD(void, Update);
    
)