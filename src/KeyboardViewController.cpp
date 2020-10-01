#include "KeyboardViewController.hpp"

#include "GlobalNamespace/UIKeyboard.hpp"
#include "UnityEngine/Resources.hpp"

#include "BeatSaberUI.hpp"

DEFINE_EVENT(CustomUITest::KeyboardViewController, System::Action_1<Il2CppString*>*, confirmPressed);

DEFINE_EVENT(CustomUITest::KeyboardViewController, System::Action*, cancelPressed);

DEFINE_EVENT(CustomUITest::KeyboardViewController, System::Action_1<Il2CppString*>*, textChanged);

void TextKeyWasPressedEvent(CustomUITest::KeyboardViewController* self, char c) {
    self->inputString = il2cpp_utils::createcsstr(to_utf8(csstrtostr(self->inputString)) + std::string({c}));
    self->UpdateInputText();
}

void DeleteButtonWasPressedEvent(CustomUITest::KeyboardViewController* self) {
    if(self->inputString->get_Length() > 0)
        self->inputString = self->inputString->Substring(0, self->inputString->get_Length()-1);
    self->UpdateInputText();
}

void OkButtonWasPressedEvent(CustomUITest::KeyboardViewController* self) {
    if(self->confirmPressed)
        self->confirmPressed->Invoke(self->inputString);
}

void CancelButtonWasPressedEvent(CustomUITest::KeyboardViewController* self) {
    if(self->cancelPressed)
        self->cancelPressed->Invoke();
}

void CustomUITest::KeyboardViewController::DidActivate(bool firstActivation, HMUI::ViewController::ActivationType activationType) {
    inputString = il2cpp_utils::createcsstr("");
    if(activationType == HMUI::ViewController::ActivationType::AddedToHierarchy && firstActivation) {
        inputText = BeatSaberUI::CreateText(get_rectTransform(), "", UnityEngine::Vector2(0.0f, 22.0f));
        inputText->set_alignment(TMPro::TextAlignmentOptions::Center);
        inputText->set_fontSize(6.0f);
        GlobalNamespace::UIKeyboard* kb = ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::UIKeyboard*>(), [](GlobalNamespace::UIKeyboard* x) { return to_utf8(csstrtostr(x->get_name())) != "CustomUIKeyboard"; });
        
        keyboardGO = UnityEngine::Object::Instantiate(kb, get_rectTransform(), false)->get_gameObject();

        UnityEngine::Object::Destroy(keyboardGO->GetComponent<GlobalNamespace::UIKeyboard*>());

        keyboard = keyboardGO->AddComponent<CustomUITest::CustomUIKeyboard*>();
        keyboard->add_textKeyWasPressedEvent(il2cpp_utils::MakeAction<System::Action_1<::Il2CppChar>>(il2cpp_functions::class_get_type(classof(System::Action_1<::Il2CppChar>*)), this, TextKeyWasPressedEvent));
        keyboard->add_deleteButtonWasPressedEvent(il2cpp_utils::MakeAction<System::Action>(il2cpp_functions::class_get_type(classof(System::Action*)), this, DeleteButtonWasPressedEvent));
        keyboard->add_okButtonWasPressedEvent(il2cpp_utils::MakeAction<System::Action>(il2cpp_functions::class_get_type(classof(System::Action*)), this, OkButtonWasPressedEvent));
        keyboard->add_cancelButtonWasPressedEvent (il2cpp_utils::MakeAction<System::Action>(il2cpp_functions::class_get_type(classof(System::Action*)), this, CancelButtonWasPressedEvent));
    } else {
        UpdateInputText();
    }
    inputText->set_text(il2cpp_utils::createcsstr("..."));
}

void CustomUITest::KeyboardViewController::UpdateInputText() {
    if(inputText){
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

void CustomUITest::KeyboardViewController::ClearInputText() {
    inputString = il2cpp_utils::createcsstr("");
    UpdateInputText();
}

