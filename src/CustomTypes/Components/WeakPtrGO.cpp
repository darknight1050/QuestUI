#include "CustomTypes/Components/WeakPtrGO.hpp"

DEFINE_TYPE(QuestUI::inner, WeakPtrGOComponent)

QuestUI::WeakPtrHolder::ComponentMap QuestUI::WeakPtrHolder::goComponentMap = {};

namespace QuestUI::inner {
    using namespace QuestUI;

    void WeakPtrGOComponent::Init(void *innerPtr) {
        if (!innerPtrSet) {
            innerPtrSet = PtrSet::New_ctor();
        }

        this->innerPtrSet->Add(il2cpp_utils::cast<Il2CppObject>(innerPtr));
        WeakPtrHolder::goComponentMap[this].emplace((void*) innerPtr);
    }

    void WeakPtrGOComponent::OnDestroy() {
        this->~WeakPtrGOComponent();
    }

    WeakPtrGOComponent::~WeakPtrGOComponent() {
        doDestroy();
    }

    void WeakPtrGOComponent::doDestroy() {
        WeakPtrHolder::goComponentMap.erase(this);
    }
}