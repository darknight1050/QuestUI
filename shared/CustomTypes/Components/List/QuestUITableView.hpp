#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/TableView.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, TableView, HMUI::TableView,
    DECLARE_OVERRIDE_METHOD(void, ReloadData, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::ReloadData>::get());
    DECLARE_OVERRIDE_METHOD(void, DidSelectCellWithIdx, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::DidSelectCellWithIdx>::get(), int idx);

    DECLARE_INSTANCE_FIELD(int, selectedRow);
    DECLARE_INSTANCE_METHOD(int, get_scrolledRow);
    DECLARE_INSTANCE_METHOD(int, get_scrollDistance);
);
