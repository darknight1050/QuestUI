#include "CustomTypes/Components/List/QuestUIBoxTableCell.hpp"

DEFINE_TYPE(QuestUI, QuestUIBoxTableCell);

namespace QuestUI
{   

	UnityEngine::Color _selectedColor0 = UnityEngine::Color(0.0f, 64.0f / 85.0f, 1.0f, 1.0f);

	UnityEngine::Color _selectedColor1 = UnityEngine::Color(0.0f, 64.0f / 85.0f, 1.0f, 0.0f);

	UnityEngine::Color _highlightedColor0 = UnityEngine::Color(0.0f, 64.0f / 85.0f, 1.0f, 1.0f);

	UnityEngine::Color _highlightedColor1 = UnityEngine::Color(0.0f, 64.0f / 85.0f, 1.0f, 0.0f);

    void QuestUIBoxTableCell::SetComponents(HMUI::ImageView* coverImage, HMUI::ImageView* selectionImage)
    {
        this->coverImage = coverImage;
        this->selectionImage = selectionImage;
    }

    void QuestUIBoxTableCell::SetData(UnityEngine::Sprite* coverSprite)
    {
        if (coverImage)
            coverImage->set_sprite(coverSprite);
    }

    void QuestUIBoxTableCell::RefreshVisuals()
    {
        if(selected || highlighted)
        {
            selectionImage->set_enabled(true);
            selectionImage->set_color0(highlighted ? _highlightedColor0 : _selectedColor0);
            selectionImage->set_color1(highlighted ? _highlightedColor1 : _selectedColor1);
        }
        else
        {
            selectionImage->set_enabled(false);
        }
    }

    void QuestUIBoxTableCell::SelectionDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        RefreshVisuals();
    }

    void QuestUIBoxTableCell::HighlightDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        RefreshVisuals();
    }

}