//
// Created by Matt on 11/19/2023.
//

#pragma once
#include "Viewport.h"

namespace pdcpp
{

    class ListBoxModel
    {
    public:
        virtual int getNumRows() = 0;
        virtual void drawItem(int rowNumber, PDRect bounds, bool isSelected) = 0;
        virtual Component* refreshRow(int rowNumber, bool isRowSelected, Component* toUpdate) = 0;
    };


    class ListBox
        : public Component
    {
    public:
        explicit ListBox(ListBoxModel* model);

        void updateContent();
        void selectRow(int rowNumber, bool display=true, bool deselectOther=true);
        int getNumSelected();
        int getSelectedRow(int index=0);

        void displayRow(int rowIndex);
        void redrawRow(int rowIndex);

        void draw() override;

    private:
        pdcpp::Viewport m_Viewport;
    };

} // pdcpp
