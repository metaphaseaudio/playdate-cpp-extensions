//
// Created by Matt on 12/26/2023.
//

#pragma once
#include "ComponentFocusView.h"
#include "pdcpp/core/SparseSet.h"

namespace pdcpp
{
    class GridView
        : public pdcpp::Component
    {
    public:
        GridView(float cellWidth, float cellHeight, float m_Padding=0);
        void refreshContent();
        [[ nodiscard ]] virtual int getNumRows() const = 0;
        [[ nodiscard ]] virtual int getNumCols() const = 0;
        void setCellFocus(int row, int column, bool shouldShowCell=true);
        [[ nodiscard ]] pdcpp::Point<int> getCellFocus() const;

        void displayCell(int row, int column);

        void setCellWidth(float width);
        void setCellHeight(float height);

    protected:
        void resized(const Rectangle<float>& newBounds) override;
        virtual Component* refreshComponentForCell(int row, int column, bool hasFocus, Component* toUpdate) = 0;

    private:
        int m_ColFocus{0}, m_RowFocus{0};
        float m_CellWidth, m_CellHeight, m_Padding;
        pdcpp::Component m_Content;
        pdcpp::ComponentFocusView m_Container;
        std::vector<std::vector<Component*>> m_Cells;
    };

}
