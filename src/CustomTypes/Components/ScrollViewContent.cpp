#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/ScrollViewContent.hpp"
#include "ArrayUtil.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/LayoutRebuilder.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

DEFINE_CLASS(QuestUI::ScrollViewContent);

void QuestUI::ScrollViewContent::Start() {
    LayoutRebuilder::ForceRebuildLayoutImmediate(GetComponent<RectTransform*>());
}

void QuestUI::ScrollViewContent::OnEnable() {
    UpdateScrollView();
}

void QuestUI::ScrollViewContent::OnRectTransformDimensionsChange() {
    UpdateScrollView();
}

void QuestUI::ScrollViewContent::UpdateScrollView() {
    if(!scrollView) return;
    scrollView->Setup();
    scrollView->RefreshButtonsInteractibility();
}
