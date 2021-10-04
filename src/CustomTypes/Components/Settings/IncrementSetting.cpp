#include <string>
#include <iomanip>
#include <sstream>

#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "ArrayUtil.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/HideFlags.hpp"

DEFINE_TYPE(QuestUI, IncrementSetting);

Il2CppString* QuestUI::IncrementSetting::GetRoundedString() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(Decimals) << CurrentValue;
    return il2cpp_utils::newcsstr(stream.str());
}

void QuestUI::IncrementSetting::DecButtonPressed() {
    CurrentValue -= Increment;
    UpdateValue();
}

void QuestUI::IncrementSetting::IncButtonPressed() {
    CurrentValue += Increment;
    UpdateValue();
}

void QuestUI::IncrementSetting::UpdateValue() {
    if(HasMin && CurrentValue < MinValue)
        CurrentValue = MinValue;
    if(HasMax && CurrentValue > MaxValue)
        CurrentValue = MaxValue;
    Text->SetText(GetRoundedString());
    if(OnValueChange)
        OnValueChange(CurrentValue);
}

