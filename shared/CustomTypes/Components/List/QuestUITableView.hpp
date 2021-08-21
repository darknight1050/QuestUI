#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/TableView.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, TableView, HMUI::TableView,
    DECLARE_OVERRIDE_METHOD(void, ReloadData, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::ReloadData>::get());
    //DECLARE_INSTANCE_METHOD(void, ReloadData);
);
