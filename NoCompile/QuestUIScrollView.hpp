#pragma once
#include "HMUI/ScrollView.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, QuestUIScrollView, HMUI::ScrollView,

    DECLARE_INSTANCE_FIELD(bool, reserveButtonSpace);

    DECLARE_METHOD(void, SetReserveButtonSpace, bool value);
    DECLARE_METHOD(bool, GetReserveButtonSpace);
    DECLARE_METHOD(void, Setup);
    
    REGISTER_FUNCTION(QuestUIScrollView,
        REGISTER_FIELD(reserveButtonSpace);

        REGISTER_METHOD(SetReserveButtonSpace);
        REGISTER_METHOD(GetReserveButtonSpace);
        REGISTER_METHOD(Setup);
    )
)