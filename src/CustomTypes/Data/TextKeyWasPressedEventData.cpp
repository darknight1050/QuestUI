#include "CustomTypes/Data/TextKeyWasPressedEventData.hpp"

DEFINE_CLASS(QuestUI::TextKeyWasPressedEventData);

void QuestUI::TextKeyWasPressedEventData::ctor(QuestUI::CustomUIKeyboard* customUIKeyboard, Il2CppChar key) {
    this->customUIKeyboard = customUIKeyboard;
    this->key = key;
}