#include <string>
#include <iomanip>
#include <sstream>

#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/IncrementSetting.hpp"
#include "ArrayUtil.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/HideFlags.hpp"

DEFINE_CLASS(QuestUI::IncrementSetting);

Il2CppString* QuestUI::IncrementSetting::GetRoundedString() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(Decimals) << CurrentValue;
    return il2cpp_utils::createcsstr(stream.str());
}

void QuestUI::IncrementSetting::DecButtonPressed() {
    CurrentValue -= Increment;
	Text->SetText(GetRoundedString());
}

void QuestUI::IncrementSetting::IncButtonPressed() {
    CurrentValue += Increment;
	Text->SetText(GetRoundedString());
}

