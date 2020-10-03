#pragma once
#include "GlobalNamespace/UIKeyboard.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, CustomUIKeyboard, GlobalNamespace::UIKeyboard,

    DECLARE_METHOD(void, Awake);

    REGISTER_FUNCTION(CustomUIKeyboard,
        REGISTER_METHOD(Awake);
    )
)