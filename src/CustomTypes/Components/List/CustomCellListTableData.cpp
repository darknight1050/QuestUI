#include "CustomTypes/Components/List/CustomCellListTableData.hpp"
#include "HMUI/Touchable.hpp"

DEFINE_TYPE(QuestUI, CustomCellListTableData);
DEFINE_TYPE(QuestUI, CustomCellTableCell);

namespace QuestUI
{
    #pragma region CustomCellListTableData 
    /* -- CustomCellListTableData -- */
    void CustomCellListTableData::ctor()
    {
        data = List<Il2CppObject*>::New_ctor();
    }

    HMUI::TableCell* CustomCellListTableData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        auto tableCell = UnityEngine::GameObject::New_ctor()->AddComponent<CustomCellTableCell*>();
        if (clickableCells)
        {
            tableCell->get_gameObject()->AddComponent<HMUI::Touchable*>();
            tableCell->set_interactable(true);
        }
        static auto QuestUICustomCellListCell_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUICustomCellListCell");
        static auto QuestUICustomTableCell_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUICustomTableCell");
        tableCell->set_reuseIdentifier(QuestUICustomCellListCell_cs);
        tableCell->set_name(QuestUICustomTableCell_cs);
        return tableCell;
    }

    float CustomCellListTableData::CellSize()
    {
        return cellSize;
    }

    int CustomCellListTableData::NumberOfCells()
    {
        return data->get_Count();
    }
    #pragma endregion

    #pragma region CustomCellTableCell 
    /* -- CustomCellTableCell -- */
    void CustomCellTableCell::ctor()
    {
        selected = List<UnityEngine::GameObject*>::New_ctor();
        hovered = List<UnityEngine::GameObject*>::New_ctor();
        neither = List<UnityEngine::GameObject*>::New_ctor();
    }

    void CustomCellTableCell::SelectionDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        RefreshVisuals();
    }

    void CustomCellTableCell::HighlightDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        RefreshVisuals();
    }

    #define UpdateGOList(list, condition) \
        int list## length = list->get_Count(); \
        for (int i = 0; i < list## length; i++) { \
            list->items->values[i]->SetActive(condition); \
        } \

    void CustomCellTableCell::RefreshVisuals()
    {
        bool isSelected = get_selected();
        bool isHighlighted = get_highlighted();

        UpdateGOList(selected, isSelected);
        UpdateGOList(hovered, isHighlighted);
        UpdateGOList(neither, !(isSelected || isHighlighted));
    }
    #pragma endregion
}