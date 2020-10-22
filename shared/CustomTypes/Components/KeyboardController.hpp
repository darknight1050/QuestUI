#pragma once
#include <sstream>
#include <string>
#include "QuestUIKeyboard.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "System/Action.hpp"
#include "System/Action_1.hpp"

#include "custom-types/shared/macros.hpp"

#include "../../eventmacros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, KeyboardController, HMUI::ViewController,

    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, keyboardGO);

    DECLARE_INSTANCE_FIELD(QuestUI::QuestUIKeyboard*, keyboard);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, inputText);

    DECLARE_INSTANCE_FIELD(Il2CppString*, inputString);


    DECLARE_EVENT(System::Action_1<Il2CppString*>*, confirmPressed);

    DECLARE_EVENT(System::Action*, cancelPressed);

    DECLARE_EVENT(System::Action_1<Il2CppString*>*, textChanged);


    DECLARE_METHOD(void, Awake);

    DECLARE_METHOD(void, UpdateInputText);

    DECLARE_METHOD(void, ClearInputText);
    
    DECLARE_METHOD(void, SetInputText, Il2CppString* text);

    REGISTER_FUNCTION(KeyboardController,
        REGISTER_FIELD(keyboardGO);
        REGISTER_FIELD(keyboard);
        REGISTER_FIELD(inputText);
        REGISTER_FIELD(inputString);

        REGISTER_EVENT(confirmPressed);
        REGISTER_EVENT(cancelPressed);
        REGISTER_EVENT(textChanged);

        REGISTER_METHOD(Awake);
        REGISTER_METHOD(UpdateInputText);
        REGISTER_METHOD(ClearInputText);
        REGISTER_METHOD(SetInputText);
    )
)