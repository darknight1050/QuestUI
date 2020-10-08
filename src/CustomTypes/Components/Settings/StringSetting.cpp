#include <string>
#include <iomanip>
#include <sstream>

#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/Settings/StringSetting.hpp"
#include "ArrayUtil.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/HideFlags.hpp"

DEFINE_CLASS(QuestUI::StringSetting);

void QuestUI::StringSetting::ConfirmPressed(Il2CppString* text) {
    CurrentValue = text->ToUpper();
    ModalView->Hide(true, nullptr);
    Text->set_text(CurrentValue);
    if(OnValueChange)
        OnValueChange->Invoke(CurrentValue);
}

void QuestUI::StringSetting::CancelPressed() {
    ModalView->Hide(true, nullptr);
}

void QuestUI::StringSetting::ButtonPressed() {
    KeyboardController->SetInputText(CurrentValue);
    ModalView->Show(true, true, nullptr);
}