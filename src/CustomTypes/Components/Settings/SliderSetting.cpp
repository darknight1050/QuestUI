#include "CustomTypes/Components/Settings/SliderSetting.hpp"
#include "custom-types/shared/delegate.hpp"

#include "UnityEngine/Time.hpp"
#include "System/Action_2.hpp"
#include "System/Collections/IEnumerator.hpp"

#include "UnityEngine/WaitForSeconds.hpp"
#include "UnityEngine/WaitForFixedUpdate.hpp"

DEFINE_TYPE(QuestUI, SliderSetting);

namespace QuestUI
{
    void SliderSetting::ctor()
    {
        INVOKE_CTOR();
    }

    void SliderSetting::set_value(float value)
    {
        if (slider) slider->set_value(value);
        if (text) text->set_text(TextForValue(value));
    }

    float SliderSetting::get_value()
    {
        if (slider) return slider->get_value();
        return 0.0f;
    }

    void SliderSetting::Setup(float min, float max, float current, float increments, float applyTime, std::function<void(float)> callback)
    {
        // if it's a whole number
        if (fabs(increments - round(increments)) < 0.000001) isInt = true;
        if (!slider) slider = GetComponentInChildren<HMUI::RangeValuesTextSlider*>();
        slider->set_minValue(min);
        slider->set_maxValue(max);
        slider->set_value(current);

        int numSteps = ((max - min) / increments) + 1;
        slider->set_numberOfSteps(numSteps);

        std::function<void(HMUI::RangeValuesTextSlider*, float)> fun = std::bind(&SliderSetting::OnChange, this, std::placeholders::_1, std::placeholders::_2);
        slider->add_valueDidChangeEvent(custom_types::MakeDelegate<System::Action_2<HMUI::RangeValuesTextSlider*, float>*>(classof(System::Action_2<HMUI::RangeValuesTextSlider*, float>*), fun));

        timerResetValue = applyTime;

        OnValueChange = callback;

        text = slider->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
    }

    void SliderSetting::OnEnable()
    {
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(SetTextOnEnable()));
    }

    custom_types::Helpers::Coroutine SliderSetting::SetTextOnEnable()
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForFixedUpdate::New_ctor());
        text->set_text(TextForValue(get_value()));
        //co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(0.1f));
        //text->set_text(TextForValue(get_value()));
        co_return;
    }

    StringW SliderSetting::TextForValue(float value)
    {
        if (this->FormatString)
            return this->FormatString(value);

        if (isInt)
            return string_format("%d", (int)round(value));
        else
            return string_format("%.2f", value);
    }

    void SliderSetting::OnChange(HMUI::RangeValuesTextSlider* _, float val)
    {
        if (text) text->set_text(TextForValue(val));
        if (timerResetValue > 0.0f)
        {
            ranCallback = false;
            timer = timerResetValue;
        }
        else
        {
            if (OnValueChange)
            {
                if (isInt)
                    OnValueChange(round(get_value()));
                else
                    OnValueChange(get_value());
            }
        }
    }

    void SliderSetting::Update()
    {
        if (timerResetValue <= 0.0f) return;
        if (timer > 0.0f) timer -= UnityEngine::Time::get_deltaTime();
        if (timer < 0.0f && !ranCallback) 
        {
            ranCallback = true;
            if (OnValueChange)
            {
                if (isInt)
                    OnValueChange((int)round(get_value()));
                else
                    OnValueChange(get_value());
            }
        }
    }
}