#include "QuestUI.hpp"
#include "CustomUIKeyboard.hpp"
#include "KeyboardViewController.hpp"

#include "custom-types/shared/register.hpp"

void QuestUI::Init() {
    static bool init = false;
    if(!init){
        init = true;
        il2cpp_functions::Init();
        custom_types::Register::RegisterType<QuestUI::CustomUIKeyboard>();
        custom_types::Register::RegisterType<QuestUI::KeyboardViewController>();
    }
}