#pragma once

#include <unordered_map>

#include "custom-types/shared/macros.hpp"

#ifndef HAS_CODEGEN
#warning "No codegen found, WeakPtrGO relies on it."
#endif

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"

// Yoinked from QuestUI components
// Why have I made such a cursed creation
// what have I created


#define DECLARE_SIMPLE_MONO_DTOR() \
void __Finalize() {                \
    Il2CppObject* l = this;                               \
    il2cpp_utils::RunMethodThrow(l, il2cpp_utils::il2cpp_type_check::MetadataGetter<&UnityEngine::Object::Finalize>::get());                               \
    this->~___TargetType();        \
} \
___CREATE_INSTANCE_METHOD(__Finalize, "__Finalize", (::il2cpp_utils::FindMethod("System", "Object", "Finalize")->flags & ~METHOD_ATTRIBUTE_ABSTRACT) | METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, ::il2cpp_utils::FindMethod("System", "Object", "Finalize"))


// A hacky version of WeakPtr
DECLARE_CLASS_CODEGEN(QuestUI::inner, WeakPtrGOComponent, UnityEngine::MonoBehaviour,
  DECLARE_INSTANCE_METHOD(void, Init, void* innerPtr);

  DECLARE_INSTANCE_METHOD(void, OnDestroy); // calls destructor
  DECLARE_SIMPLE_MONO_DTOR();
  DECLARE_DEFAULT_CTOR();

  public:
  ~WeakPtrGOComponent();

  void doDestroy();
)

namespace QuestUI {
    template<class T, class U>
    concept pointer_type_match = std::is_pointer_v<T> && std::is_convertible_v<T, U>;


    template<typename T>
    struct GetGameObject;

    template<typename T>
    requires(pointer_type_match<T*, UnityEngine::Component*>)
    struct GetGameObject<T> {
        static constexpr UnityEngine::GameObject* get(T* ptr) {
            return ptr->get_gameObject();
        }
    };

    template<>
    struct GetGameObject<UnityEngine::GameObject> {
        static constexpr UnityEngine::GameObject* get(UnityEngine::GameObject* ptr) {
            return ptr;
        }
    };

    // Encapsulation
    class WeakPtrHolder {
    public:
        using ComponentMap = std::unordered_map<QuestUI::inner::WeakPtrGOComponent const*, std::unordered_set<void*>>;

        // keepAliveGO can be gc'ed and this should still be safe
        // we are hashing the address
        static bool isPointerAlive(QuestUI::inner::WeakPtrGOComponent const* keepAliveGO, void* ptr) {
            auto it = goComponentMap.find(keepAliveGO);

            if (it == goComponentMap.end()) return false;

            return it->second.contains(ptr);
        }

    private:
        // This is modified only by WeakPtrGOComponent
        static ComponentMap goComponentMap;

        friend class QuestUI::inner::WeakPtrGOComponent;
        friend QuestUI::BeatSaber::ClearCache();
    };

    /// When the static game object is destroyed, assume all pointers associated will be set to null
    /// @remark This is designed for Unity components in mind, meaning that it will not work on just any type unless it's a Unity Component or GameObject
    ///
    /// @brief  Attaches a MonoBehaviour to an existing game object, then watches it's lifetime.
    ///         Once the attached game object is destroyed, it will remove it from tracking hashmap.
    ///         WeakPtrGO looks at the hashmap, checks if the ptr is gone and if so sets the inner ptr to null.
    ///         Essentially, WeakPtr. Note that this could have a slight performance impact
    ///
    ///         Note: operator -> does NOT check if the pointer was destroyed, it assumes you already validated
    ///         it for performance reasons. ALWAYS VALIDATE BEFORE USE
    ///
    /// @tparam T
    ///
    ///  @related What happens if the object is copied by Unity using Object.Instantiate? It probably is fine but we don't know that for sure.
    template<typename T>
    requires(pointer_type_match<T*, UnityEngine::Component*> || pointer_type_match<T*, UnityEngine::GameObject*>)
    class WeakPtrGO {
    public:
        WeakPtrGO() = default; // assign ptr fields null

        explicit WeakPtrGO(T* ptr) {
            CRASH_UNLESS(ptr);
            emplace(ptr);
        }

        void emplace(T* ptr) {
            this->ptr = ptr;

            if (!ptr)
                return;

            if (WeakPtrHolder::isPointerAlive(keepAliveGO, ptr))
                return;

            UnityEngine::GameObject* go = GetGameObject<T>::get(ptr);
            auto* comp = go->GetComponent<QuestUI::inner::WeakPtrGOComponent *>();

            if (!comp) {
                comp = go->AddComponent<QuestUI::inner::WeakPtrGOComponent *>();
            }
            CRASH_UNLESS(go); // should never happen

            keepAliveGO = comp;
            CRASH_UNLESS(comp); // should never happen

            comp->Init(ptr);
        }

        T* getInner() {
            validateInner();

            return ptr;
        }

        inline WeakPtrGO<T>& operator=(T* other) {
            emplace(other);
            return *this;
        }

        T* const operator ->() const {
            return ptr;
        }

        operator T* () {
            validateInner();

            return ptr;
        }

        operator bool() {
            validateInner();
            return (bool) ptr;
        }

        bool isValid() {
            validateInner();

            return (bool) ptr;
        }

    private:
        void validateInner() {
            if (ptr) {
                if (!WeakPtrHolder::isPointerAlive(keepAliveGO, ptr)) {
                    ptr = nullptr;
                }
            }
        }

        T* ptr = nullptr;
        QuestUI::inner::WeakPtrGOComponent const* keepAliveGO;
    };
}
