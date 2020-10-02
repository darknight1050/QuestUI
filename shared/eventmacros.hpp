#pragma once
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