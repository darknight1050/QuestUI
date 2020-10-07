#pragma once
#include "CustomTypes/Components/QuestUIKeyboard.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, TextKeyWasPressedEventData, Il2CppObject,


    DECLARE_INSTANCE_FIELD(QuestUI::QuestUIKeyboard*, questUIKeyboard);
    DECLARE_INSTANCE_FIELD(Il2CppChar, key);

    DECLARE_CTOR(ctor, QuestUI::QuestUIKeyboard* questUIKeyboard, Il2CppChar key);
    
    REGISTER_FUNCTION(TextKeyWasPressedEventData,
        REGISTER_FIELD(questUIKeyboard);
        REGISTER_FIELD(key);

        REGISTER_METHOD(ctor);
    )
)