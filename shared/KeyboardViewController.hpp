#pragma once
#include <sstream>
#include <string>
#include "CustomUIKeyboard.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_ActivationType.hpp"
#include "System/Action.hpp"
#include "System/Action_1.hpp"

#include "custom-types/shared/macros.hpp"

#ifdef DECLARE_EVENT
#error "DECLARE_EVENT is already defined! Undefine it before including macros.hpp!"
#endif
// Declares a event with the given type, and name.
// Creates the field and the add/remove methods
#define DECLARE_EVENT(type, name) \
DECLARE_INSTANCE_FIELD(type, name); \
DECLARE_METHOD(void, add_##name, type action); \
DECLARE_METHOD(void, remove_##name, type action);

#ifdef REGISTER_EVENT
#error "REGISTER_EVENT is already defined! Undefine it before including macros.hpp!"
#endif
// Registers the event with the given type, and name.
// Registers the field and the add/remove methods
#define REGISTER_EVENT(name) \
REGISTER_FIELD(name); \
REGISTER_METHOD(add_##name); \
REGISTER_METHOD(remove_##name);

#ifdef DEFINE_EVENT
#error "DEFINE_EVENT is already defined! Undefine it before including macros.hpp!"
#endif
// Defines the event in the given namespace with the given type, and name.
// Defines the add/remove methods
#define DEFINE_EVENT(namespace, type, name) \
void namespace::add_##name(type action) { \
    name = (type)System::Delegate::Combine(name, action); \
} \
void namespace::remove_##name(type action) { \
    if(name) \
        name = (type)System::Delegate::Remove(name, action); \
}

DECLARE_CLASS_CODEGEN(CustomUITest, KeyboardViewController, HMUI::ViewController,

    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, keyboardGO);

    DECLARE_INSTANCE_FIELD(CustomUITest::CustomUIKeyboard*, keyboard);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, inputText);

    DECLARE_INSTANCE_FIELD(Il2CppString*, inputString);


    DECLARE_EVENT(System::Action_1<Il2CppString*>*, confirmPressed);

    DECLARE_EVENT(System::Action*, cancelPressed);

    DECLARE_EVENT(System::Action_1<Il2CppString*>*, textChanged);


    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 2), bool firstActivation, HMUI::ViewController::ActivationType activationType);
    
    DECLARE_METHOD(void, UpdateInputText);
    DECLARE_METHOD(void, ClearInputText);

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
    )
)