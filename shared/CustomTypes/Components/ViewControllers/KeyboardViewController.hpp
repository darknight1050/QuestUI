#pragma once
#include <sstream>
#include <string>
#include "../CustomUIKeyboard.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_ActivationType.hpp"
#include "System/Action.hpp"
#include "System/Action_1.hpp"

#include "custom-types/shared/macros.hpp"

#include "../../../eventmacros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, KeyboardViewController, HMUI::ViewController,

    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, keyboardGO);

    DECLARE_INSTANCE_FIELD(QuestUI::CustomUIKeyboard*, keyboard);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, inputText);

    DECLARE_INSTANCE_FIELD(Il2CppString*, inputString);


    DECLARE_EVENT(System::Action_1<Il2CppString*>*, confirmPressed);

    DECLARE_EVENT(System::Action*, cancelPressed);

    DECLARE_EVENT(System::Action_1<Il2CppString*>*, textChanged);


    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 2), bool firstActivation, HMUI::ViewController::ActivationType activationType);
    
    DECLARE_METHOD(void, UpdateInputText);

    DECLARE_METHOD(void, ClearInputText);
    
    DECLARE_METHOD(void, SetInputText, Il2CppString* text);

    REGISTER_FUNCTION(KeyboardViewController,
        REGISTER_FIELD(keyboardGO);
        REGISTER_FIELD(keyboard);
        REGISTER_FIELD(inputText);
        REGISTER_FIELD(inputString);

        REGISTER_EVENT(confirmPressed);
        REGISTER_EVENT(cancelPressed);
        REGISTER_EVENT(textChanged);

        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(UpdateInputText);
        REGISTER_METHOD(ClearInputText);
        REGISTER_METHOD(SetInputText);
    )
)