#include "CustomTypes/Components/List/QuestUITableView.hpp"
#include "UnityEngine/Vector2.hpp"

DEFINE_TYPE(QuestUI, TableView);

namespace QuestUI
{
    void TableView::ReloadData()
    {
        this->HMUI::TableView::ReloadData();
        if (tableType == HMUI::TableView::TableType::Horizontal)
        {
            contentTransform->set_anchorMin({0.0f, 0.0f});
            contentTransform->set_anchorMax({0.0f, 1.0f});
        }
    }
}