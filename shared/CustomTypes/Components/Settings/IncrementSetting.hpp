#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, IncrementSetting, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(int, Decimals);
    DECLARE_INSTANCE_FIELD(float, Increment);
    DECLARE_INSTANCE_FIELD(float, CurrentValue);
    DECLARE_INSTANCE_FIELD(bool, HasMin);
    DECLARE_INSTANCE_FIELD(bool, HasMax);
    DECLARE_INSTANCE_FIELD(float, MinValue);
    DECLARE_INSTANCE_FIELD(float, MaxValue);
    DECLARE_INSTANCE_FIELD(UnityEngine::Events::UnityAction_1<float>*, OnValueChange);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, Text);

    DECLARE_METHOD(Il2CppString*, GetRoundedString);
    DECLARE_METHOD(void, IncButtonPressed);
    DECLARE_METHOD(void, DecButtonPressed);
    DECLARE_METHOD(void, UpdateValue);

    REGISTER_FUNCTION(IncrementSetting,
        REGISTER_FIELD(Decimals);
        REGISTER_FIELD(Increment);
        REGISTER_FIELD(CurrentValue);
        REGISTER_FIELD(HasMin);
        REGISTER_FIELD(HasMax);
        REGISTER_FIELD(MinValue);
        REGISTER_FIELD(MaxValue);
        REGISTER_FIELD(OnValueChange);
        REGISTER_FIELD(Text);

        REGISTER_METHOD(GetRoundedString);
        REGISTER_METHOD(DecButtonPressed);
        REGISTER_METHOD(IncButtonPressed);
        REGISTER_METHOD(UpdateValue);
    )
)