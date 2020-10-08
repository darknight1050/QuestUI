#include "CustomTypes/Components/KeyboardController.hpp"

#include "GlobalNamespace/UIKeyboard.hpp"
#include "UnityEngine/Resources.hpp"

#include "BeatSaberUI.hpp"

DEFINE_CLASS(QuestUI::KeyboardController);

DEFINE_EVENT(QuestUI::KeyboardController, System::Action_1<Il2CppString*>*, confirmPressed);

DEFINE_EVENT(QuestUI::KeyboardController, System::Action*, cancelPressed);

DEFINE_EVENT(QuestUI::KeyboardController, System::Action_1<Il2CppString*>*, textChanged);

void TextKeyWasPressedEvent(QuestUI::KeyboardController* self, char c) {
    self->inputString = il2cpp_utils::createcsstr(to_utf8(csstrtostr(self->inputString)) + std::string({c}));
    self->UpdateInputText();
}

void DeleteButtonWasPressedEvent(QuestUI::KeyboardController* self) {
    if(self->inputString->get_Length() > 0)
        self->inputString = self->inputString->Substring(0, self->inputString->get_Length()-1);
    self->UpdateInputText();
}

void QuestUI::KeyboardController::Awake() {
    if(!inputString)
        inputString = il2cpp_utils::createcsstr("...");
    inputText = BeatSaberUI::CreateText(get_rectTransform(), "", UnityEngine::Vector2(0.0f, 14.0f));
    inputText->set_alignment(TMPro::TextAlignmentOptions::Center);
    inputText->set_fontSize(8.0f);
    GlobalNamespace::UIKeyboard* kb = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::UIKeyboard*>(), [](GlobalNamespace::UIKeyboard* x) { return to_utf8(csstrtostr(x->get_name())) != "QuestUIKeyboard"; });
        
    keyboardGO = UnityEngine::Object::Instantiate(kb, get_rectTransform(), false)->get_gameObject();

    UnityEngine::Object::Destroy(keyboardGO->GetComponent<GlobalNamespace::UIKeyboard*>());

    keyboard = keyboardGO->AddComponent<QuestUI::QuestUIKeyboard*>();
    keyboard->add_textKeyWasPressedEvent(il2cpp_utils::MakeAction<System::Action_1<Il2CppChar>>(il2cpp_functions::class_get_type(classof(System::Action_1<Il2CppChar>*)), this, TextKeyWasPressedEvent));
    keyboard->add_deleteButtonWasPressedEvent(il2cpp_utils::MakeAction<System::Action>(il2cpp_functions::class_get_type(classof(System::Action*)), this, DeleteButtonWasPressedEvent));
    keyboard->add_okButtonWasPressedEvent(il2cpp_utils::MakeAction<System::Action>(il2cpp_functions::class_get_type(classof(System::Action*)), this, +[](QuestUI::KeyboardController* self) { if(self->confirmPressed) self->confirmPressed->Invoke(self->inputString); } ));
    keyboard->add_cancelButtonWasPressedEvent (il2cpp_utils::MakeAction<System::Action>(il2cpp_functions::class_get_type(classof(System::Action*)), this, +[](QuestUI::KeyboardController* self) { if(self->cancelPressed) self->cancelPressed->Invoke(); }));
    
    UpdateInputText();
}

void QuestUI::KeyboardController::UpdateInputText() {
    if(inputText) {
        inputText->set_text(inputString->ToUpper());
        if (System::String::IsNullOrEmpty(inputString)) {
            keyboard->set_enableOkButtonInteractivity(false);
        }
        else {
            keyboard->set_enableOkButtonInteractivity(true);
        }
        if(textChanged)
            textChanged->Invoke(inputString);
    }
}

void QuestUI::KeyboardController::ClearInputText() {
    inputString = il2cpp_utils::createcsstr("");
    UpdateInputText();
}


void QuestUI::KeyboardController::SetInputText(Il2CppString* text) {
    inputString = text;
    UpdateInputText();
}

