#include "CustomTypes/Components/List/QuestUITableView.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Rect.hpp"
#include "System/Collections/Generic/HashSet_1.hpp"

DEFINE_TYPE(QuestUI, TableView);

namespace QuestUI
{
    void TableView::ReloadData()
    {
        // reload data with correct anchors
        auto reload = il2cpp_utils::FindMethodUnsafe("HMUI", "TableView", "ReloadData", 0);
        il2cpp_utils::RunMethod(this, reload);
        if (tableType == HMUI::TableView::TableType::Horizontal)
        {
            contentTransform->set_anchorMin({0.0f, 0.0f});
            contentTransform->set_anchorMax({0.0f, 1.0f});
        }
    }

    int TableView::get_selectedRow()
    {
        if (reinterpret_cast<System::Collections::ICollection*>(selectedCellIdxs)->get_Count() <= 0) return -1;
        auto enumerator = selectedCellIdxs->GetEnumerator();
        if (!enumerator.MoveNext()) return -1;
        return enumerator.current;
    }

    void TableView::DidSelectCellWithIdx(int idx)
    {
        // keep track of which idx is selected
        auto DidSelect = il2cpp_utils::FindMethodUnsafe("HMUI", "TableView", "DidSelectCellWithIdx", 1);
        il2cpp_utils::RunMethod(this, DidSelect, idx);
    }

    int TableView::get_scrolledRow()
    {
        // pos / cellsize is where we are right now, if this is ever larger than numcells - 1 just return the latter tho
        auto pos = get_contentTransform()->get_anchoredPosition();
        return std::min((int)(pos.y / get_cellSize()), get_numberOfCells() - 1);
    }

    int TableView::get_scrollDistance()
    {
        // height / cellsize is amount of cells that fit within the viewport, and thus the scroll distance
        return get_viewportTransform()->get_rect().get_height() / get_cellSize();
    }
}