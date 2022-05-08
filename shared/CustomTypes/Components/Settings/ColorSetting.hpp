#pragma once
#include "UnityEngine/MonoBehaviour.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "../ModalColorPicker.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, ColorSetting, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, editButton);
    DECLARE_INSTANCE_FIELD(QuestUI::ModalColorPicker*, colorPickerModal);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, colorImage);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, _currentColor);

    DECLARE_INSTANCE_METHOD(UnityEngine::Color, get_currentColor);
    DECLARE_INSTANCE_METHOD(void, set_currentColor, UnityEngine::Color);

    DECLARE_INSTANCE_METHOD(bool, get_interactable);
    DECLARE_INSTANCE_METHOD(void, set_interactable, bool);

    DECLARE_CTOR(ctor);
    public:
      void Setup(QuestUI::ModalColorPicker* _colorPickerModal, UnityEngine::Color initialColor, std::function<void(UnityEngine::Color)> onChangeCallback, std::function<void(UnityEngine::Color)> onDoneCallback, std::function<void()> onCancelCallback);
      std::function<void(UnityEngine::Color)> onChange = nullptr;
      std::function<void(UnityEngine::Color)> onDone = nullptr;
      std::function<void()> onCancel = nullptr;
)