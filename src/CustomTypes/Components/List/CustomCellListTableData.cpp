#include "CustomTypes/Components/List/CustomCellListTableData.hpp"
#include "CustomTypes/Components/List/CustomCellListWrapper.hpp"
#include "CustomTypes/Components/List/QuestUITableView.hpp"
#include "HMUI/Touchable.hpp"

DEFINE_TYPE(QuestUI, CustomCellListTableData);
DEFINE_TYPE(QuestUI, CustomCellTableCell);

extern Logger& getLogger();

namespace QuestUI
{
    #pragma region CustomCellListTableData 
    /* -- CustomCellListTableData -- */
    void CustomCellListTableData::ctor()
    {
        INVOKE_CTOR();
        cellSize = 8.5f;
        clickableCells = true;
    }

    void CustomCellListTableData::dtor()
    {
        if (listWrapper) delete listWrapper;
        Finalize();
    }

    HMUI::TableCell* CustomCellListTableData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        // no dequeueing as of yet, needs to be properly handled which it is not yet at this point
        auto tableCell = UnityEngine::GameObject::New_ctor()->AddComponent<CustomCellTableCell*>();
        if (listWrapper) listWrapper->SetupCell(tableCell, idx);

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
        return listWrapper ? listWrapper->GetDataSize() : 0;
    }
    #pragma endregion

    #pragma region CustomCellTableCell 
    /* -- CustomCellTableCell -- */
    void CustomCellTableCell::ctor()
    {
        selectedGroup = List<UnityEngine::GameObject*>::New_ctor();
        hoveredGroup = List<UnityEngine::GameObject*>::New_ctor();
        neitherGroup = List<UnityEngine::GameObject*>::New_ctor();
    }

    void CustomCellTableCell::SelectionDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        getLogger().info("Selection Changed");
        RefreshVisuals();
        // if we are now selected
        if (get_selected())
        {
            QuestUI::TableView* tableView = reinterpret_cast<QuestUI::TableView*>(get_tableCellOwner());
            CustomCellListTableData* dataSource = reinterpret_cast<CustomCellListTableData*>(tableView->get_dataSource());
            if (dataSource->listWrapper) dataSource->listWrapper->OnCellSelected(this, idx);
        }
    }

    void CustomCellTableCell::HighlightDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        getLogger().info("Highlight Changed");
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

        UpdateGOList(selectedGroup, isSelected);
        UpdateGOList(hoveredGroup, isHighlighted);
        UpdateGOList(neitherGroup, !(isSelected || isHighlighted));
    }
    #pragma endregion
}