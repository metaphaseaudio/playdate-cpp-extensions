/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/26/2023
 *  Original author: MrBZapp
 */

#include <cassert>
#include "pdcpp/components/GridView.h"


pdcpp::GridView::GridView() { addChildComponent(&m_Container); }

void pdcpp::GridView::refreshContent()
{
    m_Container.clearFocusView();
    m_Cells.resize(getNumRows());
    const auto bounds = getBounds();
    auto verticalOffset = 0;

    for (int rowI = 0; rowI < getNumRows(); rowI++)
    {
        m_Cells.at(rowI).resize(getNumCols());
        const auto height = getRowHeight(rowI) == 0 ? bounds.height : getRowHeight(rowI);
        auto horizontalOffset = 0;
        for (int colI = 0; colI < getNumCols(); colI++)
        {
            const auto width = getColWidth(colI) == 0 ? bounds.width : getColWidth(colI);
            auto* comp = refreshComponentForCell(rowI, colI, m_RowFocus == rowI && m_ColFocus == colI, m_Cells.at(rowI).at(colI));

            // You must return a component, even if you want it to be blank!
            assert(comp != nullptr);
            m_Cells.at(rowI).at(colI) = comp;

            comp->setBounds(pdcpp::Rectangle<float>(horizontalOffset, verticalOffset, width, height));
            m_Container.addChildToFocusContainer(comp);
            horizontalOffset += width;
        }
        verticalOffset += height;
    }
}


void pdcpp::GridView::resized(const pdcpp::Rectangle<float>& newBounds)
{
    m_Container.setBounds(newBounds);
    refreshContent();
}

void pdcpp::GridView::setCellFocus(int row, int column, bool shouldShowCell, bool limit)
{
    if (getNumRows() == 0 || getNumCols() == 0) { return; }

    m_RowFocus = limit ? pdcpp::limit(0, getNumRows() - 1, row) : row;
    m_ColFocus = limit ? pdcpp::limit(0, getNumCols() - 1, column) : column;

    // Refresh here to let the component update based on the new focus
    refreshContent();

    if (shouldShowCell)
        { displayCell(m_RowFocus, m_ColFocus); }
}

void pdcpp::GridView::displayCell(int row, int column)
{
    row = pdcpp::limit(0, getNumRows() - 1, row);
    column = pdcpp::limit(0, getNumCols() - 1, column);
    m_Container.bringComponentIntoView(row * getNumCols() + column);
}

pdcpp::Point<int> pdcpp::GridView::getCellFocus() const
{
    return {m_ColFocus, m_RowFocus};
}

void pdcpp::GridView::scrollX(int px)
{
    m_Container.moveContentBy(px, 0, true);
}

void pdcpp::GridView::scrollY(int px)
{
    m_Container.moveContentBy(0, px, true);
}

