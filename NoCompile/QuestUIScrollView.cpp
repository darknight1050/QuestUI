#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/QuestUIScrollView.hpp"
#include "ArrayUtil.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/HideFlags.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "HMUI/VerticalScrollIndicator.hpp"
#include "System/Math.hpp"

using namespace UnityEngine;

DEFINE_CLASS(QuestUI::QuestUIScrollView);

bool QuestUI::QuestUIScrollView::GetReserveButtonSpace() {
    return reserveButtonSpace;
}

void QuestUI::QuestUIScrollView::SetReserveButtonSpace(bool value) {
    reserveButtonSpace = value;
    viewport->set_sizeDelta(UnityEngine::Vector2(-13.0f, reserveButtonSpace ? -20.0f : -8.0f));
}

void QuestUI::QuestUIScrollView::Setup() {
    if(!contentRectTransform) return;
    contentHeight = contentRectTransform->GetChild(0)->GetComponent<RectTransform*>()->get_rect().get_height();
    float viewPortHeight = viewport->get_rect().get_height();
    scrollPageHeight = viewPortHeight;
    bool active = contentHeight > viewPortHeight;
    pageUpButton->get_gameObject()->SetActive(active);
    pageDownButton->get_gameObject()->SetActive(active);
    if (verticalScrollIndicator)
    {
        verticalScrollIndicator->get_gameObject()->SetActive(active);
        verticalScrollIndicator->set_normalizedPageHeight(viewPortHeight / contentHeight);
    }
    ComputeScrollFocusPosY();

    RectTransform* handle = verticalScrollIndicator->handle;
    handle->set_sizeDelta(UnityEngine::Vector2(handle->get_sizeDelta().x, abs(handle->get_sizeDelta().y)));
}

