#include "CustomTypes/Components/ModalColorPicker.hpp"

DEFINE_TYPE(QuestUI, ModalColorPicker);

extern Logger& getLogger();

namespace QuestUI
{
    void ModalColorPicker::ctor()
    {
        INVOKE_CTOR();
    }

    UnityEngine::Color ModalColorPicker::get_color()
    {
        return currentColor;
    }

    void ModalColorPicker::set_color(UnityEngine::Color value)
    {
        currentColor = value;
        if (rgbPanel)
            rgbPanel->set_color(currentColor);
        if (hsvPanel && hsvPanel->get_color() != currentColor) //If you're wondering why we check this for hsv it's so that if color is one where changing hue has no effect it won't lock up the hue slider
            hsvPanel->set_color(currentColor);
        if (colorImage)
            colorImage->set_color(currentColor);
    }
    
    void ModalColorPicker::Setup(UnityEngine::Color init, std::function<void(UnityEngine::Color)> onChangeCallback, std::function<void(UnityEngine::Color)> onDoneCallback, std::function<void()> onCancelCallback)
    {
        currentColor = init;
        onChange = onChangeCallback;
        onDone = onDoneCallback;
        onCancel = onCancelCallback;
    }

    void ModalColorPicker::Show()
    {
        getLogger().info("showing modal: %p", modalView);
        if (modalView) modalView->Show(true, true, nullptr);
    }
    
    void ModalColorPicker::CancelPressed()
    {
        if (onCancel) onCancel();
        if (modalView) modalView->Hide(true, nullptr);
    }

    void ModalColorPicker::DonePressed()
    {   
        if (onDone) onDone(currentColor);
        if (modalView) modalView->Hide(true, nullptr);
    }

    void ModalColorPicker::OnChange(UnityEngine::Color color, GlobalNamespace::ColorChangeUIEventType type)
    {
        if (onChange) onChange(color);
        currentColor = color;
    }
}