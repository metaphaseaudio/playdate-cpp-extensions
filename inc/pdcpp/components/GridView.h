/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/26/2023
 *  Original author: MrBZapp
 */
#pragma once
#include "ComponentFocusView.h"
#include "pdcpp/core/SparseSet.h"

namespace pdcpp
{
    class GridView
        : public pdcpp::Component
    {
    public:
        /**
         * Constructs a GridView Component, which tries to replicate many of the
         * same properties as the Lua GridView object, allowing callers to
         * display different components in each cell of a grid, and navigate
         * around said grid.
         */
        GridView();

        // Default virtual destructor
        virtual ~GridView() = default;

        /**
         * Call this when the actual Components within cells have changed.
         * Re-drawing handles redrawing all child cells, so this is
         * specifically to indicate that Components need to be replaced.
         */
        void refreshContent();

        /**
         * Pure virtual. Sub-class this GridView, and provide a number of Rows
         * in the grid.
         */
        [[ nodiscard ]] virtual int getNumRows() const = 0;

        /**
         * Pure virtual. Sub-class this GridView, and provide a number of
         * Columns in the grid.
         */
        [[ nodiscard ]] virtual int getNumCols() const = 0;

        /**
         * Pure virtual. Sub-class this GridView, and provide a width for the
         * row index in pixels. This can be a constant value or change per row.
         * Return 0 to have each row fill the viewport of the Gridview.
         */
        [[ nodiscard ]] virtual int getRowHeight(int i) const = 0;

        /**
         * Pure virtual. Sub-class this GridView, and provide a width for the
         * column index in pixels. This can be a constant value or change per
         * column. Return 0 to have each column fill the viewport of the
         * GridView
         */
        [[ nodiscard ]] virtual int getColWidth(int i) const = 0;

        /**
         * Sets the cell at a given row/column as being in user focus.
         * Optionally, if the cell is not currently in the Viewport, move that
         * cell into the viewport.
         *
         * @param row the row of the cell for focus
         * @param column the column of the cell for focus
         * @param shouldShowCell whether or not to move the Viewport to show the
         *     cell. On by default.
         */
        void setCellFocus(int row, int column, bool shouldShowCell=true);

        /**
         * @returns the row and column numbers currently focused as a Point.
         */
        [[ nodiscard ]] pdcpp::Point<int> getCellFocus() const;

        /**
         * Moves the viewport such that a cell is visible. Does nothing if the
         * cell is already visible
         *
         * @param row the row to bring into the view
         * @param column the column to bring into the view
         */
        void displayCell(int row, int column);

        /**
         * Scroll the view by n pixels on the X axis
         * @param px the number of pixels to scroll
         */
        void scrollX(int px);

        /**
         * Scroll the view by n pixels on the Y axis
         * @param px the number of pixels to scroll
         */
        void scrollY(int px);

    protected:
        /**
         * Override this to provide content for each of the GridView's cells.
         * It will be called during `refreshContent`. Note that the GridView
         * will not take ownership of the Component, so the user must manage
         * its lifecycle.
         *
         * @param row the row of the cell to return
         * @param column the column of the cell to return
         * @param hasFocus indicate whether this cell has focus
         * @param toUpdate the current Component in the cell, if any. Use this
         *     to either update the current component, or return a new one.
         * @return the component which will be displayed in the specified cell.
         *     if no updates are needed, you can return the exact same pointer
         *     as `toUpdate`.
         */
        virtual Component* refreshComponentForCell(int row, int column, bool hasFocus, Component* toUpdate) = 0;

        // Overrides base class method
        void resized(const Rectangle<float>& newBounds) override;

    private:
        int m_ColFocus{0}, m_RowFocus{0};
        pdcpp::Component m_Content;
        pdcpp::ComponentFocusView m_Container;
        std::vector<std::vector<Component*>> m_Cells;
    };

}
