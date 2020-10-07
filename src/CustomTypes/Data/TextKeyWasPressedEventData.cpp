
#include "CustomTypes/Data/TextKeyWasPressedEventData.hpp"

DEFINE_CLASS(QuestUI::TextKeyWasPressedEventData);

void QuestUI::TextKeyWasPressedEventData::ctor(QuestUI::QuestUIKeyboard* questUIKeyboard, Il2CppChar key) {
    this->questUIKeyboard = questUIKeyboard;
    this->key = key;
}
