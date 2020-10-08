#pragma once

#include "../KeyboardController.hpp"
 
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ModalView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, StringSetting, UnityEngine::MonoBehaviour,

	DECLARE_INSTANCE_FIELD(Il2CppString*, CurrentValue);
    DECLARE_INSTANCE_FIELD(UnityEngine::Events::UnityAction_1<Il2CppString*>*, OnValueChange);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, Text);
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, ModalView);
    DECLARE_INSTANCE_FIELD(QuestUI::KeyboardController*, KeyboardController);

    DECLARE_METHOD(void, ConfirmPressed, Il2CppString* text);
    DECLARE_METHOD(void, CancelPressed);
    DECLARE_METHOD(void, ButtonPressed);

    REGISTER_FUNCTION(StringSetting,
		REGISTER_FIELD(CurrentValue);
		REGISTER_FIELD(OnValueChange);
		REGISTER_FIELD(Text);
		REGISTER_FIELD(ModalView);
        REGISTER_FIELD(KeyboardController);

        REGISTER_METHOD(ConfirmPressed);
        REGISTER_METHOD(CancelPressed);
        REGISTER_METHOD(ButtonPressed);
    )
)