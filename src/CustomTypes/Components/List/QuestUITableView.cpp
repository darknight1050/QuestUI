#include "CustomTypes/Components/List/QuestUITableView.hpp"
#include "UnityEngine/Vector2.hpp"

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
}