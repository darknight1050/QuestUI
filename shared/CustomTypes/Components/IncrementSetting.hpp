#pragma once
#include "codegen/include/UnityEngine/MonoBehaviour.hpp"
#include "codegen/include/TMPro/TextMeshProUGUI.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, IncrementSetting, UnityEngine::MonoBehaviour,

	DECLARE_INSTANCE_FIELD(int, Decimals);
	DECLARE_INSTANCE_FIELD(float, Increment);
	DECLARE_INSTANCE_FIELD(float, CurrentValue);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, Text);

    DECLARE_METHOD(void, IncButtonPressed);
    DECLARE_METHOD(void, DecButtonPressed);
    DECLARE_METHOD(Il2CppString*, GetRoundedString);

    REGISTER_FUNCTION(IncrementSetting,
		REGISTER_FIELD(Decimals);
		REGISTER_FIELD(Increment);
		REGISTER_FIELD(CurrentValue);
        REGISTER_FIELD(Text);

        REGISTER_METHOD(DecButtonPressed);
        REGISTER_METHOD(IncButtonPressed);
        REGISTER_METHOD(GetRoundedString);
    )
)