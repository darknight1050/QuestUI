#pragma once
#include "GlobalNamespace/IRefreshable.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_INTERFACES(QuestUI, CustomPanelController, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject), { classof(GlobalNamespace::IRefreshable*) },

    DECLARE_OVERRIDE_METHOD(void, Refresh, il2cpp_utils::FindMethod("", "IRefreshable", "Refresh"));

)
