#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/ModalView.hpp"
#include "GlobalNamespace/RGBPanelController.hpp"
#include "GlobalNamespace/HSVPanelController.hpp"
#include "GlobalNamespace/ColorChangeUIEventType.hpp"
#include "UnityEngine/UI/Image.hpp"

#include <functional>

DECLARE_CLASS_CODEGEN(QuestUI, ModalColorPicker, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, modalView);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::RGBPanelController*, rgbPanel);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::HSVPanelController*, hsvPanel);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, colorImage);

    DECLARE_INSTANCE_FIELD(UnityEngine::Color, currentColor);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, openColor);

    DECLARE_INSTANCE_METHOD(UnityEngine::Color, get_color);
    DECLARE_INSTANCE_METHOD(void, set_color, UnityEngine::Color value);
    
    DECLARE_INSTANCE_METHOD(void, Show);
    DECLARE_INSTANCE_METHOD(void, CancelPressed);
    DECLARE_INSTANCE_METHOD(void, DonePressed);
    DECLARE_INSTANCE_METHOD(void, OnChange, UnityEngine::Color color, GlobalNamespace::ColorChangeUIEventType type);

    DECLARE_CTOR(ctor);
    public:
        void Setup(UnityEngine::Color init, std::function<void(UnityEngine::Color)> onChangeCallback, std::function<void(UnityEngine::Color)> onDoneCallback, std::function<void()> onCancelCallback);
        std::function<void(UnityEngine::Color)> onChange = nullptr;
        std::function<void(UnityEngine::Color)> onDone = nullptr;
        std::function<void()> onCancel = nullptr;
)