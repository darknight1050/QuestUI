
#include "CustomTypes/Data/CustomDataType.hpp"

DEFINE_CLASS(QuestUI::CustomDataType);

void QuestUI::CustomDataType::ctor() {
    this->data = nullptr;
}

QuestUI::CustomDataType* QuestUI::CustomDataType::SetDataPointer(void* data) {
    Finalize();
    this->data = data;
    return this;
}

void QuestUI::CustomDataType::Finalize(){
    if(this->allocated){
        free(this->data);
        this->data = nullptr;
        this->allocated = false;
    }
}