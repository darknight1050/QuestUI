#pragma once
#include "GlobalNamespace/UIKeyboard.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, QuestUIKeyboard, GlobalNamespace::UIKeyboard,

    DECLARE_METHOD(void, Awake);

    REGISTER_FUNCTION(QuestUIKeyboard,
        REGISTER_METHOD(Awake);
    )
)