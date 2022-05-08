#include "CustomTypes/Components/Settings/ColorSetting.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"

#include <utility>

#define MakeDelegate(DelegateType, varName) (il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

DEFINE_TYPE(QuestUI, ColorSetting);

UnityEngine::Color QuestUI::ColorSetting::get_currentColor() {
    return _currentColor;
}

void QuestUI::ColorSetting::set_currentColor(UnityEngine::Color color) {
    _currentColor = color;
    if(colorImage) {
        colorImage->set_color(color);
    }
}

void QuestUI::ColorSetting::set_interactable(bool interactable) {
    if(editButton) {
        editButton->set_interactable(interactable);
    }
}

bool QuestUI::ColorSetting::get_interactable() {
    if(editButton) {
        return editButton->get_interactable();
    }
    return false;
}

void QuestUI::ColorSetting::ctor() {
    INVOKE_CTOR();
}

void QuestUI::ColorSetting::Setup(ModalColorPicker* _colorPickerModal, UnityEngine::Color initialColor, std::function<void(UnityEngine::Color)> onChangeCallback, std::function<void(UnityEngine::Color)> onDoneCallback, std::function<void()> onCancelCallback) {
    colorPickerModal = _colorPickerModal;

    set_currentColor(initialColor);

    onChange = std::move(onChangeCallback);
    onCancel = std::move(onCancelCallback);
    onDone = std::move(onDoneCallback);

    std::function<void()> onClick = [this]() {
        colorPickerModal->Show();
    };

    editButton->get_onClick()->AddListener(MakeDelegate(UnityEngine::Events::UnityAction*, onClick));

    colorPickerModal->onCancel = this->onCancel;
    colorPickerModal->onChange = this->onChange;
    colorPickerModal->onDone = [this] (UnityEngine::Color color) {
        this->set_currentColor(color);
        if(this->onDone)
            this->onDone(color);
    };
}