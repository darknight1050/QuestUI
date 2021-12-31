#include "CustomTypes/Components/ClickableText.hpp"

DEFINE_TYPE(QuestUI, ClickableText);

using namespace UnityEngine;

namespace QuestUI
{
    void ClickableText::ctor()
    {
        INVOKE_CTOR();
        onClickEvent = OnPointerClickEvent();
        pointerEnterEvent = OnPointerEnterEvent();
        pointerExitEvent = OnPointerExitEvent();
        isHighlighted = false;
        highlightColor = UnityEngine::Color(0.60f, 0.80f, 1.0f, 1.0f);
        defaultColor = UnityEngine::Color(1.0f, 1.0f, 1.0f, 1.0f);

        static auto base_ctor = il2cpp_utils::FindMethod(classof(HMUI::CurvedTextMeshPro*), ".ctor");
        if (base_ctor)
        {
            il2cpp_utils::RunMethod(this, base_ctor);
        }
    }

    void ClickableText::OnPointerClick(EventSystems::PointerEventData* eventData)
    {
        set_isHighlighted(false);
        onClickEvent.invoke(eventData);
    }

    void ClickableText::OnPointerEnter(EventSystems::PointerEventData* eventData)
    {
        set_isHighlighted(true);
        pointerEnterEvent.invoke(eventData);
    }

    void ClickableText::OnPointerExit(EventSystems::PointerEventData* eventData)
    {
        set_isHighlighted(false);
        pointerExitEvent.invoke(eventData);
    }

    void ClickableText::UpdateHighlight()
    {
        set_color(get_isHighlighted() ? get_highlightColor() : get_defaultColor());
    }

    void ClickableText::set_highlightColor(UnityEngine::Color color)
    {
        highlightColor = color;
        UpdateHighlight();
    }

    UnityEngine::Color ClickableText::get_highlightColor()
    {
        return highlightColor;
    }

    void ClickableText::set_defaultColor(UnityEngine::Color color)
    {
        defaultColor = color;
        UpdateHighlight();
    }

    UnityEngine::Color ClickableText::get_defaultColor()
    {
        return defaultColor;
    }

    bool ClickableText::get_isHighlighted()
    {
        return isHighlighted;
    }

    void ClickableText::set_isHighlighted(bool value)
    {
        isHighlighted = value;
        UpdateHighlight();
    }

    ClickableText::OnPointerClickEvent& ClickableText::get_onPointerClickEvent()
    {
        return onClickEvent;
    }

    ClickableText::OnPointerEnterEvent& ClickableText::get_onPointerEnterEvent()
    {
        return pointerEnterEvent;
    }

    ClickableText::OnPointerExitEvent& ClickableText::get_onPointerExitEvent()
    {
        return pointerExitEvent;
    }
}