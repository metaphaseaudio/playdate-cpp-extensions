//
// Created by Matt on 12/26/2023.
//

#include "pdcpp/components/GridView.h"


pdcpp::GridView::GridView(float cellWidth, float cellHeight, float padding)
    : m_CellWidth(cellWidth)
    , m_CellHeight(cellHeight)
    , m_Padding(padding)
{
    addChildComponent(&m_Container);
}

void pdcpp::GridView::refreshContent()
{
    m_Container.clearFocusView();
    m_Cells.resize(getNumRows());
    for (int rowI = 0; rowI < getNumRows(); rowI++)
    {
        m_Cells.at(rowI).resize(getNumCols());
        for (int colI = 0; colI < getNumCols(); colI++)
        {
            const auto bounds = getBounds();
            auto* comp = refreshComponentForCell(rowI, colI, m_RowFocus == rowI && m_ColFocus == colI, m_Cells.at(rowI).at(colI));
            comp->setBounds(pdcpp::Rectangle<float>(
                colI * m_CellWidth,
                rowI * m_CellHeight,
                m_CellWidth == 0 ? bounds.width : m_CellWidth,
                m_CellHeight == 0 ? bounds.height : m_CellHeight
            ));
            m_Container.addChildToFocusContainer(comp);
        }
    }
}


void pdcpp::GridView::resized(const pdcpp::Rectangle<float>& newBounds)
{
    m_Container.setBounds(newBounds);
    refreshContent();
}

void pdcpp::GridView::setCellFocus(int row, int column, bool shouldShowCell)
{
    m_RowFocus = row;
    m_ColFocus = column;
    if (shouldShowCell)
        { displayCell(row, column); }
}

void pdcpp::GridView::displayCell(int row, int column)
{
    m_Container.bringComponentIntoView(row + column * getNumRows());
    refreshContent();
}

pdcpp::Point<int> pdcpp::GridView::getCellFocus() const
{
    return {m_ColFocus, m_RowFocus};
}

