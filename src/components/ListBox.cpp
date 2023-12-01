//
// Created by Matt on 11/19/2023.
//

#include <cassert>
#include <cmath>
#include <memory>
#include <algorithm>

#include "pdcpp/components/ListBox.h"


pdcpp::ListBox::RowComponent::RowComponent(pdcpp::ListBox& lb)
    : p_Owner(lb)
{}

void pdcpp::ListBox::RowComponent::draw()
{
    if (auto* m = p_Owner.getListBoxModel())
        { m->drawItem(getRow(), getBounds(), false, isSelected()); }
}

void pdcpp::ListBox::RowComponent::updateRowAndSelection(const int newRow, const bool nowSelected)
{
    const auto rowChanged = std::exchange(m_Row, newRow) != newRow;
    const auto selectionChanged = std::exchange(m_Selected, nowSelected) != nowSelected;

    if (rowChanged || selectionChanged)
    {
        // TODO: This really should redraw the row, and *only* the row. However,
        //   the way the component system is currently structured means drawing
        //   here will draw this row in some random scope. We have to rely on
        //   the parents getting re-drawn to ensure this.
    }
}

void pdcpp::ListBox::RowComponent::update(const int newRow, const bool nowSelected)
{
    updateRowAndSelection(newRow, nowSelected);

    if (auto* m = p_Owner.getListBoxModel())
    {
        p_CustomComponent.reset (m->refreshComponentForRow(newRow, false, nowSelected, p_CustomComponent.release()));

        if (p_CustomComponent != nullptr)
        {
            addChildComponent(p_CustomComponent.get());
            p_CustomComponent->setBounds(getBounds());
        }
    }
}

void pdcpp::ListBox::RowComponent::resized(PDRect newBounds)
{
    if (p_CustomComponent != nullptr)
        { p_CustomComponent->setBounds(newBounds); }
}

////////////////////////////////////////////////////////////////////////////////
pdcpp::ListBox::ListBox(pdcpp::ListBoxModel* model)
    : p_Model(model)
{
    m_RowView.setContent(&m_Content);
    addChildComponent(&m_RowView);
    updateContent();
}

void pdcpp::ListBox::updateContent()
{
    m_HasUpdated = true;
    const auto rowH = getRowHeight();

    if (rowH > 0)
    {
        auto y = m_RowView.getViewPosition().getY();
        auto w = m_RowView.getBounds().width;
        auto h = getBounds().height;
        m_Content.setBounds({0, 0, w, float(rowH * getNumRows())});

        const auto numNeeded = (size_t) (4 + h / rowH);
        m_Rows.resize(std::min(numNeeded, m_Rows.size()));

        while (numNeeded > m_Rows.size())
        {
            m_Rows.emplace_back(std::make_unique<RowComponent>(*this));
            m_Content.addChildComponent(m_Rows.back().get());
        }

        m_FirstIndex = y / rowH;
        m_FirstWholeIndex = (y + rowH - 1) / rowH;
        m_LastWholeIndex = (y + h - 1) / rowH;

        const auto startIndex = getIndexOfFirstVisibleRow();
        const auto lastIndex = startIndex + (int) m_Rows.size();

        for (auto row = startIndex; row < lastIndex; ++row)
        {
            if (auto* rowComp = getComponentForRowIfOnscreen(row))
            {
                rowComp->setBounds({0, float(row * rowH), w, float(rowH)});
                rowComp->update(row, isRowSelected(row));
            }
            else
            {
                assert(false);
            }
        }
    }
    m_RowView.redraw();
}

void pdcpp::ListBox::selectRow(int row, bool dontScroll, bool deselectOthers)
{
    if (!m_MultipleSelection)
        { deselectOthers = true; }

    if (isRowSelected(row) && !(deselectOthers && getNumSelected() > 1))
        { return; }

    const auto bounds = getBounds();
    if (bounds.height == 0 || bounds.width == 0)
        { dontScroll = true; }

    const auto totalRows = p_Model != nullptr ? p_Model->getNumRows() : 0;

    if (deselectOthers)
        { m_Selected.clear(); }

        m_Selected.insert(row);

    const auto height = bounds.height;
    m_HasUpdated = false;

    if (row < m_FirstWholeIndex && !dontScroll)
        { m_RowView.setViewPosition(0, row * m_RowHeight); }

    else if (row >= m_LastWholeIndex && !dontScroll)
    {
        const int rowsOnScreen = m_LastWholeIndex - m_FirstWholeIndex;

        if (row >= m_LastRowSelected + rowsOnScreen && rowsOnScreen < totalRows - 1)
        {
            m_RowView.setViewPosition(0, pdcpp::limit(0, std::max(0, totalRows - rowsOnScreen), row) * m_RowHeight);
        }
        else
        {
            m_RowView.setViewPosition(0, std::max(0.0f, (row  + 1) * m_RowHeight - height));
        }
    }

    if (!m_HasUpdated)
        { updateContent(); }

    m_LastRowSelected = row;
}

int pdcpp::ListBox::getNumSelected() const { return m_Selected.size(); }

int pdcpp::ListBox::getSelectedRow(int index) const { return m_Selected[index]; }

void pdcpp::ListBox::setRowHeight(int height)
{
    m_RowHeight = height;
    updateContent();
}

bool pdcpp::ListBox::isRowSelected(int rowIndex) const
    { return std::any_of(m_Selected.begin(), m_Selected.end(), [rowIndex](auto x) { return x == rowIndex; } ); }

void pdcpp::ListBox::deselectRow(int rowNumber)
{
    m_Selected.erase(rowNumber);
    updateContent();
}

bool pdcpp::ListBox::isRowVisible(int rowIndex) const
{
    const auto position = m_RowView.getViewPosition();
    auto topIndex = float(position.getY()) / float(m_RowHeight);
    auto bottomIndex = topIndex + getBounds().height;
    return rowIndex >= ::ceilf(topIndex) && rowIndex  <= ::floorf(bottomIndex);
}

void pdcpp::ListBox::resized(PDRect newBounds)
{
    m_RowView.setBounds(newBounds);
    updateContent();
}

bool pdcpp::ListBox::updateAnimation() { return m_RowView.updateAnimation(); }

int pdcpp::ListBox::getNumRows() const { return p_Model->getNumRows(); }

int pdcpp::ListBox::getRowHeight() const { return m_RowHeight; }

void pdcpp::ListBox::scrollToEnsureRowIsOnscreen(int row)
{
    const auto height = getBounds().height;

    if (row < m_FirstWholeIndex)
    {
        m_RowView.setViewPosition(0, row * m_RowHeight);
    }
    else if (row >= m_LastWholeIndex)
    {
        m_RowView.setViewPosition(0, std::max(0.0f, (row  + 1) * m_RowHeight - height));
    }
}

void pdcpp::ListBox::updateVisibleArea(const bool forceUpdate)
{
    m_HasUpdated = false;
    const auto height = getBounds().height;

    auto viewportBounds = getBounds();
    auto currentBounds = m_Content.getBounds();
    auto newX = currentBounds.x;
    auto newY = currentBounds.y;
    auto newW = viewportBounds.width;
    auto newH = getListBoxModel()->getNumRows() * getRowHeight();

    if (newY + newH < height && newH > height)
        newY = height - newH;

    m_Content.setBounds({newX, newY, newW, float(newH)});

    if (forceUpdate && !m_HasUpdated)
        { updateContent(); }
}

pdcpp::ListBox::RowComponent* pdcpp::ListBox::getComponentForRowIfOnscreen(int row) const noexcept
{
    const auto startIndex = getIndexOfFirstVisibleRow();

    return (startIndex <= row && row < startIndex + (int) m_Rows.size())
        ? m_Rows[(size_t) (row % std::max (1, int(m_Rows.size())))].get()
        : nullptr;
}
