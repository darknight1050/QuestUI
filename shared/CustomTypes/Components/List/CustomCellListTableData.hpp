#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableView_IDataSource.hpp"
#include "HMUI/TableCell.hpp"
#include "HMUI/SelectableCell.hpp"

namespace QuestUI {
    class CustomListWrapper;
}

//DECLARE_CLASS_CODEGEN_INTERFACES_FIX(QuestUI, CustomCellListTableData, UnityEngine::MonoBehaviour, { classof(HMUI::TableView::IDataSource*) },
___DECLARE_TYPE_WRAPPER_INHERITANCE(QuestUI, CustomCellListTableData, Il2CppTypeEnum::IL2CPP_TYPE_CLASS, UnityEngine::MonoBehaviour, "QuestUI", { classof(HMUI::TableView::IDataSource*) }, 0, nullptr,
    DECLARE_INSTANCE_FIELD(Il2CppString*, cellTemplate);
    DECLARE_INSTANCE_FIELD(float, cellSize);
    DECLARE_INSTANCE_FIELD(HMUI::TableView*, tableView);
    DECLARE_INSTANCE_FIELD(bool, clickableCells);
    
    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);

    DECLARE_OVERRIDE_METHOD(HMUI::TableCell*, CellForIdx, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::CellForIdx>::get(), HMUI::TableView* tableView, int idx);
    DECLARE_OVERRIDE_METHOD(float, CellSize, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::CellSize>::get());
    DECLARE_OVERRIDE_METHOD(int, NumberOfCells, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::NumberOfCells>::get());

    public:
        CustomListWrapper* listWrapper = nullptr;
)

DECLARE_CLASS_CODEGEN(QuestUI, CustomCellTableCell, HMUI::TableCell,
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, selected);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, hovered);
    DECLARE_INSTANCE_FIELD(List<UnityEngine::GameObject*>*, neither);

    DECLARE_CTOR(ctor);

    DECLARE_OVERRIDE_METHOD(void, SelectionDidChange, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::SelectableCell::SelectionDidChange>::get(), HMUI::SelectableCell::TransitionType transitionType);
    DECLARE_OVERRIDE_METHOD(void, HighlightDidChange, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::SelectableCell::HighlightDidChange>::get(), HMUI::SelectableCell::TransitionType transitionType);

    DECLARE_INSTANCE_METHOD(void, RefreshVisuals);
)

