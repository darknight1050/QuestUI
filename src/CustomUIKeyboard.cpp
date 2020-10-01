#include <sstream>
#include <string>

#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomUIKeyboard.hpp"
#include "ArrayUtil.hpp"

#include "GlobalNamespace/TextMeshProButton.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"
#include "UnityEngine/UI/Navigation.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "System/Action.hpp"
#include "System/Action_1.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace UnityEngine::UI;

struct OnTextKeyWasPressedEventData {
    CustomUITest::CustomUIKeyboard* customUIKeyboard;
    char key;
};

void OnTextKeyWasPressedEvent(OnTextKeyWasPressedEventData* data, Button* button) {
    if(data->customUIKeyboard->textKeyWasPressedEvent)
        data->customUIKeyboard->textKeyWasPressedEvent->Invoke(data->key);
}

void OnDeleteButtonWasPressedEvent(CustomUITest::CustomUIKeyboard* customUIKeyboard, Button* button) {
    if(customUIKeyboard->deleteButtonWasPressedEvent)
        customUIKeyboard->deleteButtonWasPressedEvent->Invoke();
}

void OnCancelButtonWasPressedEvent(CustomUITest::CustomUIKeyboard* customUIKeyboard, Button* button) {
    if(customUIKeyboard->cancelButtonWasPressedEvent)
        customUIKeyboard->cancelButtonWasPressedEvent->Invoke();
}

void OnOkButtonWasPressedEvent(CustomUITest::CustomUIKeyboard* customUIKeyboard, Button* button) {
    if(customUIKeyboard->okButtonWasPressedEvent)
        customUIKeyboard->okButtonWasPressedEvent->Invoke();
}

void CustomUITest::CustomUIKeyboard::Awake() {
    set_name(il2cpp_utils::createcsstr("CustomUIKeyboard"));
    keyButtonPrefab = ArrayUtil::First(Resources::FindObjectsOfTypeAll<TextMeshProButton*>(), [](TextMeshProButton* x){ return to_utf8(csstrtostr(x->get_name())) == "KeyboardButton"; });
    std::string array[] = { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "a", "s", "d", "f", "g", "h", "j", "k", "l", "z", "x", "c", "v", "b", "n", "m", "<-", "space", "OK", "Cancel" };
    unsigned long arrayLength = sizeof(array)/sizeof(std::string);
    for (int i = 0; i < arrayLength; i++)
    {
        RectTransform* parent = (RectTransform*)get_transform()->GetChild(i);
        TextMeshProButton* textMeshProButton = parent->GetComponentInChildren<TextMeshProButton*>();
        textMeshProButton->get_text()->set_text(il2cpp_utils::createcsstr(array[i]));
        RectTransform* rectTransform = (RectTransform*)textMeshProButton->get_transform();
        rectTransform->set_localPosition(UnityEngine::Vector3::get_zero());
        rectTransform->set_localScale(UnityEngine::Vector3::get_one());
        rectTransform->set_anchoredPosition(UnityEngine::Vector2::get_zero());
        rectTransform->set_anchorMin(UnityEngine::Vector2::get_zero());
        rectTransform->set_anchorMax(UnityEngine::Vector2::get_one());
        rectTransform->set_offsetMin(UnityEngine::Vector2::get_zero());
        rectTransform->set_offsetMax(UnityEngine::Vector2::get_zero());
        Navigation navigation = textMeshProButton->get_button()->get_navigation();
        navigation.set_mode(Navigation::Mode::None);
        textMeshProButton->get_button()->set_navigation(navigation);
        textMeshProButton->get_button()->set_onClick(Button::ButtonClickedEvent::New_ctor());
        if (i < arrayLength - 4)
        {
            OnTextKeyWasPressedEventData* data = new OnTextKeyWasPressedEventData();
            data->customUIKeyboard = this;
            data->key = array[i].front();
            textMeshProButton->get_button()->get_onClick()->AddListener(il2cpp_utils::MakeAction<Events::UnityAction>(il2cpp_functions::class_get_type(classof(Events::UnityAction*)), data, OnTextKeyWasPressedEvent));
        }
        else if (i == arrayLength - 4)
        {
            textMeshProButton->get_button()->get_onClick()->AddListener(il2cpp_utils::MakeAction<Events::UnityAction>(il2cpp_functions::class_get_type(classof(Events::UnityAction*)), this, OnDeleteButtonWasPressedEvent));
        }
        else if (i == arrayLength - 1)
        {   
            rectTransform->set_sizeDelta(UnityEngine::Vector2(16.0f, 0.0f));
            rectTransform->set_anchoredPosition(UnityEngine::Vector2(11.0f, 0.0f));
            textMeshProButton->get_button()->get_onClick()->AddListener(il2cpp_utils::MakeAction<Events::UnityAction>(il2cpp_functions::class_get_type(classof(Events::UnityAction*)), this, OnCancelButtonWasPressedEvent));
        }
        else if (i == arrayLength - 2)
        {   
            rectTransform->set_sizeDelta(UnityEngine::Vector2(7.0f, 0.0f));
            rectTransform->set_anchoredPosition(UnityEngine::Vector2(14.0f, 0.0f));
            okButton = textMeshProButton->get_button();
            okButton->set_interactable(okButtonInteractivity);
            textMeshProButton->get_button()->get_onClick()->AddListener(il2cpp_utils::MakeAction<Events::UnityAction>(il2cpp_functions::class_get_type(classof(Events::UnityAction*)), this, OnOkButtonWasPressedEvent));
        }
        else
        {
            rectTransform->set_anchoredPosition(UnityEngine::Vector2(10.0f, 0.0f));
            OnTextKeyWasPressedEventData* data = new OnTextKeyWasPressedEventData();
            data->customUIKeyboard = this;
            data->key = ' ';
            textMeshProButton->get_button()->get_onClick()->AddListener(il2cpp_utils::MakeAction<Events::UnityAction>(il2cpp_functions::class_get_type(classof(Events::UnityAction*)), data, OnTextKeyWasPressedEvent));
        }
    }
    for (int i = 1; i <= 11; i++)
    {
        TextMeshProButton* textButton = Object::Instantiate(keyButtonPrefab);
        std::string key = {i == 11 ? '_' : std::to_string(i).back()};

        textButton->get_text()->set_text(il2cpp_utils::createcsstr(key));
        OnTextKeyWasPressedEventData* data = new OnTextKeyWasPressedEventData();
        data->customUIKeyboard = this;
        data->key = key.front();
        textButton->get_button()->get_onClick()->AddListener(il2cpp_utils::MakeAction<Events::UnityAction>(il2cpp_functions::class_get_type(classof(Events::UnityAction*)), data, OnTextKeyWasPressedEvent));
        
        RectTransform* buttonRect = textButton->GetComponent<RectTransform*>();
        RectTransform* component2 = get_transform()->GetChild(i == 11 ? 9 : i - 1)->get_gameObject()->GetComponent<RectTransform*>();

        RectTransform* buttonHolder = Object::Instantiate(component2, component2->get_parent(), false);
        Object::Destroy(buttonHolder->GetComponentInChildren<Button*>()->get_gameObject());
        UnityEngine::Vector2 anchoredPosition = buttonHolder->get_anchoredPosition();
        anchoredPosition.x -= i == 11 ? -5.0f : 5.0f;
        anchoredPosition.y -= -10.0f;
        buttonHolder->set_anchoredPosition(anchoredPosition);

        buttonRect->SetParent(buttonHolder, false);

        buttonRect->set_localPosition(UnityEngine::Vector3::get_zero());
        buttonRect->set_localScale (UnityEngine::Vector3::get_one());
        buttonRect->set_anchoredPosition(UnityEngine::Vector2::get_zero());
        buttonRect->set_anchorMin(UnityEngine::Vector2::get_zero());
        buttonRect->set_anchorMax(UnityEngine::Vector2::get_one());
        buttonRect->set_offsetMin(UnityEngine::Vector2::get_zero());
        buttonRect->set_offsetMax(UnityEngine::Vector2::get_zero());

    }
}
