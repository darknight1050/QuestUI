#pragma once

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_INTERFACES(QuestUI, CustomPanelController, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject), il2cpp_utils::GetClassFromName("", "IRefreshable"),

    DECLARE_OVERRIDE_METHOD(void, Refresh, il2cpp_utils::FindMethod("", "IRefreshable", "Refresh"));

    REGISTER_FUNCTION(MyCustomBeatmapLevelPackCollection,
        REGISTER_METHOD(Refresh);
    )
)
