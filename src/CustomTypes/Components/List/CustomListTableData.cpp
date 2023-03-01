#include "CustomTypes/Components/List/CustomListTableData.hpp"

#include "GlobalNamespace/LayoutWidthLimiter.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "ArrayUtil.hpp"

DEFINE_TYPE(QuestUI, CustomListTableData);

using namespace HMUI;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace QuestUI 
{
    void CustomListTableData::ctor()
    {
        INVOKE_CTOR();
        set_listStyle(CustomListTableData::ListStyle::List);
        expandCell = false;
        reuseIdentifier = StringW("QuestUIListTableCell");
        tableView = nullptr;
    }
    
    void CustomListTableData::dtor()
    {
        Finalize();
    }

    void CustomListTableData::set_listStyle(CustomListTableData::ListStyle value)
    {
        this->listStyle = value;
        switch (value)
        {
            case ListStyle::List:
                cellSize = 8.5f;
                break;
            case ListStyle::Box:
                cellSize = tableView->get_tableType() == TableView::TableType::Horizontal ? 30.0f : 35.0f;
                break;
            case ListStyle::Simple:
                cellSize = 8.0f;
                break;
        }
    }

    CustomListTableData::ListStyle CustomListTableData::get_listStyle()
    {
        return listStyle;
    }

    GlobalNamespace::LevelListTableCell* CustomListTableData::GetTableCell()
    {
        auto tableCell = reinterpret_cast<GlobalNamespace::LevelListTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));
        if (!tableCell)
        {
            if (!songListTableCellInstance)
                songListTableCellInstance = Resources::FindObjectsOfTypeAll<GlobalNamespace::LevelListTableCell*>().First([](auto x){ return x->get_name() == "LevelListTableCell"; });

            tableCell = Instantiate(songListTableCellInstance);
        }

        //tableCell.SetField("_beatmapCharacteristicImages", new Image[0]);
        tableCell->notOwned = false;

        tableCell->set_reuseIdentifier(reuseIdentifier);
        return tableCell;   
    }

    QuestUIBoxTableCell* CustomListTableData::GetBoxTableCell()
    {
        auto tableCell = reinterpret_cast<QuestUIBoxTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));
        if (!tableCell)
        {
            if (!levelPackTableCellInstance)
                levelPackTableCellInstance = Resources::FindObjectsOfTypeAll<GlobalNamespace::LevelPackCell*>().First([](auto x){ return x->get_name() == "AnnotatedBeatmapLevelCollectionCell"; });
            tableCell = InstantiateBoxTableCell(levelPackTableCellInstance);
        }
        tableCell->set_reuseIdentifier(reuseIdentifier);
        return tableCell;
    }

    QuestUIBoxTableCell* CustomListTableData::InstantiateBoxTableCell(GlobalNamespace::LevelPackCell* levelPackTableCell)
    {
        levelPackTableCell = Instantiate(levelPackTableCell);
        ImageView* coverImage = levelPackTableCell->coverImage;
        ImageView* selectionImage = levelPackTableCell->selectionImage;

        auto transform = coverImage->get_transform();
        for (int i = 0; i < transform->GetChildCount(); i++)
        {
            Object::Destroy(transform->GetChild(i)->get_gameObject());
        }

        GameObject* cellObject = levelPackTableCell->get_gameObject();
        Object::Destroy(levelPackTableCell);
        QuestUIBoxTableCell* boxTableCell = cellObject->AddComponent<QuestUIBoxTableCell*>();
        boxTableCell->SetComponents(coverImage, selectionImage);
        return boxTableCell;
    }

    GlobalNamespace::SimpleTextTableCell* CustomListTableData::GetSimpleTextTableCell()
    {
        auto tableCell = reinterpret_cast<GlobalNamespace::SimpleTextTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));
        if (!tableCell)
        {
            if (!simpleTextTableCellInstance)
                simpleTextTableCellInstance = Resources::FindObjectsOfTypeAll<GlobalNamespace::SimpleTextTableCell*>().First([](auto x){ return x->get_name() == "SimpleTextTableCell"; });
            tableCell = Instantiate(simpleTextTableCellInstance);
        }

        tableCell->set_reuseIdentifier(reuseIdentifier);
        return tableCell;
    }

    HMUI::TableCell* CustomListTableData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        switch (listStyle)
        {
            case ListStyle::List: {
                auto tableCell = GetTableCell();

                TextMeshProUGUI* nameText = tableCell->songNameText;
                TextMeshProUGUI* authorText = tableCell->songAuthorText;
                tableCell->songBpmText->get_gameObject()->SetActive(false);
                tableCell->songDurationText->get_gameObject()->SetActive(false);
                tableCell->favoritesBadgeImage->get_gameObject()->SetActive(false);
                static ConstString BpmIcon("BpmIcon");
                tableCell->get_transform()->Find(BpmIcon)->get_gameObject()->SetActive(false);

                // new stuff in 1.28.0 that needs to be disabled
                tableCell->updatedBadgeGo->SetActive(false);
                tableCell->promoBadgeGo->SetActive(false);
                tableCell->promoBackgroundGo->SetActive(false);
                tableCell->layoutWidthLimiter->set_limitWidth(false);

                if (expandCell)
                {
                    nameText->get_rectTransform()->set_anchorMax(Vector2(2, 0.5f));
                    authorText->get_rectTransform()->set_anchorMax(Vector2(2, 0.5f));
                }

                auto& cellInfo = data[idx];
                nameText->set_text(cellInfo.get_text());
                authorText->set_text(cellInfo.get_subText());
                tableCell->coverImage->set_sprite(cellInfo.get_icon());

                return tableCell;
            }
            case ListStyle::Box: {
                auto& cellInfo = data[idx];
                QuestUIBoxTableCell* cell = GetBoxTableCell();
                cell->SetData(cellInfo.get_icon());

                return cell;
            }
            case ListStyle::Simple: {
                auto simpleCell = GetSimpleTextTableCell();
                simpleCell->text->set_richText(true);
                simpleCell->text->set_enableWordWrapping(true);
                simpleCell->set_text(data[idx].get_text());

                return simpleCell;
            }
        }

        return nullptr;        
    }

    float CustomListTableData::CellSize()
    {
        return cellSize;
    }

    int CustomListTableData::NumberOfCells()
    {
        return data.size();
    }

    StringW CustomListTableData::CustomCellInfo::get_text() 
    { 
        return text; 
    }

    StringW CustomListTableData::CustomCellInfo::get_subText() 
    { 
        return subText; 
    }

    StringW CustomListTableData::CustomCellInfo::get_combinedText()
    {
        return string_format("%s\n%s", text.c_str(), subText.c_str()); 
    }

    UnityEngine::Sprite* CustomListTableData::CustomCellInfo::get_icon()
    {
        if (icon) return icon;
        auto texture = Texture2D::get_blackTexture();
        return Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f,0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
    }
}