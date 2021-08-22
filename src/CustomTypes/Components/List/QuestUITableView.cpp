#include "CustomTypes/Components/List/QuestUITableView.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Rect.hpp"

DEFINE_TYPE(QuestUI, TableView);

namespace QuestUI
{
    void TableView::ReloadData()
    {
        auto reload = il2cpp_utils::FindMethodUnsafe("HMUI", "TableView", "ReloadData", 0);
        il2cpp_utils::RunMethod(this, reload);
        if (tableType == HMUI::TableView::TableType::Horizontal)
        {
            contentTransform->set_anchorMin({0.0f, 0.0f});
            contentTransform->set_anchorMax({0.0f, 1.0f});
        }
    }

    void TableView::DidSelectCellWithIdx(int idx)
    {
        auto DidSelect = il2cpp_utils::FindMethodUnsafe("HMUI", "TableView", "DidSelectCellWithIdx", 1);
        il2cpp_utils::RunMethod(this, DidSelect, idx);
        selectedRow = idx;
    }

    int TableView::get_scrolledRow()
    {
        auto pos = get_contentTransform()->get_anchoredPosition();
        return std::min((int)(pos.y / get_cellSize()), get_numberOfCells() - 1);
    }

    int TableView::get_scrollDistance()
    {
        return get_viewportTransform()->get_rect().get_height() / get_cellSize();
    }
}