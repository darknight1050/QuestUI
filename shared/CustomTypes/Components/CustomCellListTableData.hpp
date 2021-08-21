#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableView_IDataSource.hpp"
#include "HMUI/TableCell.hpp"
#include "HMUI/SelectableCell.hpp"

#define DECLARE_CLASS_CODEGEN_INTERFACES_FIX(namespaze, name, baseT, interfaceTs, ...) \
___DECLARE_TYPE_WRAPPER_INHERITANCE(namespaze, name, Il2CppTypeEnum::IL2CPP_TYPE_CLASS, baseT, #namespaze, interfaceTs, 0, nullptr, __VA_ARGS__)

#define GET_FIND_METHOD(mPtr) il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::get()

namespace QuestUI::CustomCellList {
    static std::vector<Il2CppClass*> GetCustomCellListTableDataInterfaces() {
        return { classof(HMUI::TableView::IDataSource*) };
    }
}
//QuestUI::CustomCellList::GetCustomCellListTableDataInterfaces()

DECLARE_CLASS_CODEGEN_INTERFACES_FIX(QuestUI, CustomCellListTableData, UnityEngine::MonoBehaviour, QuestUI::CustomCellList::GetCustomCellListTableDataInterfaces(),
    DECLARE_INSTANCE_FIELD(List<Il2CppObject*>*, data);
    DECLARE_INSTANCE_FIELD(Il2CppString*, cellTemplate);
    DECLARE_INSTANCE_FIELD(float, cellSize);
    DECLARE_INSTANCE_FIELD(HMUI::TableView*, tableView);
    DECLARE_INSTANCE_FIELD(bool, clickableCells);
    
    DECLARE_CTOR(ctor);

    DECLARE_OVERRIDE_METHOD(HMUI::TableCell*, CellForIdx, GET_FIND_METHOD(&HMUI::TableView::IDataSource::CellForIdx), HMUI::TableView* tableView, int idx);
    DECLARE_OVERRIDE_METHOD(float, CellSize, GET_FIND_METHOD(&HMUI::TableView::IDataSource::CellSize));
    DECLARE_OVERRIDE_METHOD(int, NumberOfCells, GET_FIND_METHOD(&HMUI::TableView::IDataSource::NumberOfCells));
)

DECLARE_CLASS_CODEGEN(QuestUI, CustomCellTableCell, HMUI::TableCell,
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, selected);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, hovered);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, neither);

    DECLARE_CTOR(ctor);

    DECLARE_OVERRIDE_METHOD(void, SelectionDidChange, GET_FIND_METHOD(&HMUI::SelectableCell::SelectionDidChange), HMUI::SelectableCell::TransitionType transitionType);
    DECLARE_OVERRIDE_METHOD(void, HighlightDidChange, GET_FIND_METHOD(&HMUI::SelectableCell::HighlightDidChange), HMUI::SelectableCell::TransitionType transitionType);

    DECLARE_INSTANCE_METHOD(void, RefreshVisuals);
)