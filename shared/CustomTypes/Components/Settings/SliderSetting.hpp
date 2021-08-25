#pragma once

#include "UnityEngine/MonoBehaviour.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "HMUI/RangeValuesTextSlider.hpp"

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, SliderSetting, UnityEngine::MonoBehaviour,
    DECLARE_CTOR(ctor);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, text);
    DECLARE_INSTANCE_FIELD(HMUI::RangeValuesTextSlider*, slider);
    DECLARE_INSTANCE_FIELD(bool, isInt);
    DECLARE_INSTANCE_FIELD(float, increments);
    DECLARE_INSTANCE_FIELD(float, timerResetValue);
    DECLARE_INSTANCE_FIELD(float, timer);
    DECLARE_INSTANCE_FIELD(bool, ranCallback);

    DECLARE_INSTANCE_METHOD(void, set_value, float value);
    DECLARE_INSTANCE_METHOD(float, get_value);

    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, OnEnable);

    DECLARE_INSTANCE_METHOD(Il2CppString*, TextForValue, float value);
    DECLARE_INSTANCE_METHOD(void, OnChange, HMUI::RangeValuesTextSlider* _, float val);

    public:
        void Setup(float min, float max, float current, float increments, float applyTime, std::function<void(float)> callback);
        std::function<void(float)> OnValueChange = nullptr;
        custom_types::Helpers::Coroutine SetTextOnEnable();
)