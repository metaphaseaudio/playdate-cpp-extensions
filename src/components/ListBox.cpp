//
// Created by Matt on 11/19/2023.
//

#include "pdcpp/components/ListBox.h"

class ComponentContainer
    : public pdcpp::Component
{
public:
    ComponentContainer();

};


pdcpp::ListBox::ListBox(pdcpp::ListBoxModel* model)
{

}

void pdcpp::ListBox::updateContent()
{

}

void pdcpp::ListBox::selectRow(int rowNumber, bool display, bool deselectOther)
{
    rowNumber = rowNumber < 0 ? 0 : rowNumber;


}

int pdcpp::ListBox::getNumSelected()
{
    return 0;
}

int pdcpp::ListBox::getSelectedRow(int index)
{
    return 0;
}

void pdcpp::ListBox::displayRow(int rowIndex)
{

}

void pdcpp::ListBox::redrawRow(int rowIndex)
{

}


void pdcpp::ListBox::draw()
{

}
