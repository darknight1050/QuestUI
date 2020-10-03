#pragma once
#include "CustomTypes/Components/CustomUIKeyboard.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, TextKeyWasPressedEventData, Il2CppObject,


    DECLARE_INSTANCE_FIELD(QuestUI::CustomUIKeyboard*, customUIKeyboard);
    DECLARE_INSTANCE_FIELD(Il2CppChar, key);

    DECLARE_CTOR(ctor, QuestUI::CustomUIKeyboard* customUIKeyboard, Il2CppChar key);
    
    REGISTER_FUNCTION(TextKeyWasPressedEventData,
        REGISTER_FIELD(customUIKeyboard);
        REGISTER_FIELD(key);

        REGISTER_METHOD(ctor);
    )
)