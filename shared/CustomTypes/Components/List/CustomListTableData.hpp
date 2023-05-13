#pragma once

#include "custom-types/shared/macros.hpp"
#include "QuestUIBoxTableCell.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Sprite.hpp"

#include "HMUI/TableView.hpp"
#include "HMUI/TableView_IDataSource.hpp"
#include "HMUI/TableCell.hpp"
#include "HMUI/SelectableCell.hpp"

#include "GlobalNamespace/LevelListTableCell.hpp"
#include "GlobalNamespace/AnnotatedBeatmapLevelCollectionCell.hpp"
#include "GlobalNamespace/SimpleTextTableCell.hpp"

#include <vector>

// this is what that used to be called, but this is more clear and understandable (Blame BSML) we just copy this
namespace GlobalNamespace {
    using LevelPackCell = AnnotatedBeatmapLevelCollectionCell;
}

DECLARE_CLASS_CODEGEN_INTERFACES(QuestUI, CustomListTableData, UnityEngine::MonoBehaviour, classof(HMUI::TableView::IDataSource*),
    public:
    enum ListStyle {
        List,
        Box,
        Simple
    };

    struct CustomCellInfo {
        std::string text;
        std::string subText;
        UnityEngine::Sprite* icon;

        CustomCellInfo(std::string text = "", std::string subText = "", UnityEngine::Sprite* icon = nullptr) : text(text), subText(subText), icon(icon) {};
        CustomCellInfo(std::string text, UnityEngine::Sprite* icon) : text(text), subText(""), icon(icon) {};

        StringW get_text();
        StringW get_subText();
        StringW get_combinedText();
        UnityEngine::Sprite* get_icon();
    };

    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelListTableCell*, songListTableCellInstance);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelPackCell*, levelPackTableCellInstance);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SimpleTextTableCell*, simpleTextTableCellInstance);

    DECLARE_INSTANCE_FIELD(StringW, reuseIdentifier);
    DECLARE_INSTANCE_FIELD(float, cellSize);
    DECLARE_INSTANCE_FIELD(HMUI::TableView*, tableView);
    DECLARE_INSTANCE_FIELD(bool, expandCell);

    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);

    DECLARE_OVERRIDE_METHOD(HMUI::TableCell*, CellForIdx, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::CellForIdx>::get(), HMUI::TableView* tableView, int idx);
    DECLARE_OVERRIDE_METHOD(float, CellSize, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::CellSize>::get());
    DECLARE_OVERRIDE_METHOD(int, NumberOfCells, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::NumberOfCells>::get());

    private:
        ListStyle listStyle = ListStyle::List;
    public:
        void set_listStyle(ListStyle value);
        ListStyle get_listStyle();

        // use push_back, or even better emplace_back to add to the data list
        std::vector<CustomCellInfo> data;

        GlobalNamespace::LevelListTableCell* GetTableCell();
        QuestUIBoxTableCell* GetBoxTableCell();
        QuestUIBoxTableCell* InstantiateBoxTableCell(GlobalNamespace::LevelPackCell* levelPackTableCell);
        GlobalNamespace::SimpleTextTableCell* GetSimpleTextTableCell();
)
