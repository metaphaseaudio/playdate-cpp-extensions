//
// Created by Matt on 11/19/2023.
//

#include <cassert>
#include <cmath>
#include <memory>
#include <algorithm>
#include <utility>

#include "pdcpp/components/ListBox.h"
#include "pdcpp/graphics/RectHelpers.h"


pdcpp::ListBox::ItemComponent::ItemComponent(pdcpp::ListBox& lb)
    : p_Owner(lb)
    , p_CustomComponent(nullptr)
{}

void pdcpp::ListBox::ItemComponent::draw()
{
    if (p_CustomComponent != nullptr) { return; };
    if (auto* m = p_Owner.getListBoxModel())
        {m->drawItem(getRow(), getBounds(), false, isSelected()); }
}

void pdcpp::ListBox::ItemComponent::updateRowAndSelection(const int newRow, const bool nowSelected)
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

void pdcpp::ListBox::ItemComponent::update(const int newRow, const bool nowSelected)
{
    updateRowAndSelection(newRow, nowSelected);

    if (auto* m = p_Owner.getListBoxModel())
    {
        p_CustomComponent = m->refreshComponentForRow(newRow, false, nowSelected, p_CustomComponent);

        if (p_CustomComponent != nullptr)
        {
            removeAllChildren();
            addChildComponent(p_CustomComponent);
            p_CustomComponent->setBounds(getBounds());
        }
    }
}

void pdcpp::ListBox::ItemComponent::resized(const pdcpp::Rectangle<float>& newBounds)
{
    if (p_CustomComponent != nullptr)
        { p_CustomComponent->setBounds(newBounds); }
}

////////////////////////////////////////////////////////////////////////////////
pdcpp::ListBox::ListBox(pdcpp::ListBoxModel* model)
    : p_Model(model)
    , selected(*this, false)
{
    m_ItemView.setContent(&m_Content);
    addChildComponent(&m_ItemView);
    updateContent();
}

void pdcpp::ListBox::updateContent()
{
    m_HasUpdated = true;
    const auto rowH = getRowHeight();

    if (rowH > 0)
    {
        auto y = m_ItemView.getViewPosition().y;
        auto w = m_ItemView.getBounds().width;
        auto h = getBounds().height;
        m_Content.setBounds({0, 0, w, float(rowH * getNumRows())});

        const auto numNeeded = (size_t) (4 + h / rowH);
        m_Items.resize(std::min(numNeeded, m_Items.size()));

        while (numNeeded > m_Items.size())
        {
            m_Items.emplace_back(std::make_unique<ItemComponent>(*this));
            m_Content.addChildComponent(m_Items.back().get());
        }

        m_FirstIndex = y / rowH;

        const auto startIndex = getIndexOfFirstVisibleRow();
        const auto lastIndex = startIndex + (int) m_Items.size();

        for (auto row = startIndex; row < lastIndex; ++row)
        {
            if (auto* rowComp = getComponentForRowIfOnscreen(row))
            {
                rowComp->setBounds({0, float(row * rowH), w, float(rowH)});
                rowComp->update(row, selected.isItemMarked(row));
            }
            else
            {
                assert(false);
            }
        }
    }
    m_ItemView.redraw();
}

void pdcpp::ListBox::setRowHeight(int height)
{
    m_RowHeight = height;
    updateContent();
}


void pdcpp::ListBox::resized(const pdcpp::Rectangle<float>& newBounds)
{
    m_ItemView.setBounds(newBounds);
    updateContent();
}

int pdcpp::ListBox::getNumRows() const { return p_Model->getNumRows(); }

int pdcpp::ListBox::getRowHeight() const { return m_RowHeight; }

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

pdcpp::ListBox::ItemComponent* pdcpp::ListBox::getComponentForRowIfOnscreen(int row) const noexcept
{
    const auto startIndex = getIndexOfFirstVisibleRow();

    return (startIndex <= row && row < startIndex + (int) m_Items.size())
        ? m_Items[(size_t) (row % std::max (1, int(m_Items.size())))].get()
        : nullptr;
}

void pdcpp::ListBox::bringItemIntoView(int itemIndex)
{
    if (isItemVisible(itemIndex)) { return; }

    auto itemBounds = m_Items[itemIndex]->getBounds();

    // Item not visible and never will be
    if (itemBounds.height == 0 || itemBounds.width == 0) { return; }

    const auto viewBounds = m_ItemView.getBounds().withOrigin({0, 0});
    const auto viewPosition = m_ItemView.getViewPosition();

    itemBounds = { itemBounds.x - viewPosition.x, itemBounds.y - viewPosition.y, itemBounds.width, itemBounds.height };

    const pdcpp::Point<float> viewBottomRight = pdcpp::RectHelpers::getBottomRight(viewBounds);
    const pdcpp::Point<float> itemBottomRight = pdcpp::RectHelpers::getBottomRight(itemBounds);

    int x = viewPosition.x, y = viewPosition.y;

    if (itemBounds.x > viewBottomRight.x)  // Move left
        { x = viewPosition.x + (itemBottomRight.x - viewBottomRight.x); }
    else if (viewBounds.x > itemBounds.x)  // Move right
        { x = itemBounds.x + viewPosition.x; }

    if (itemBottomRight.y > viewBottomRight.y)  // Move up
        { y = viewPosition.y + (itemBottomRight.y - viewBottomRight.y); }
    else if (viewBounds.y > itemBounds.y)  // Move down
        { y = itemBounds.y + viewPosition.y; }

    m_ItemView.setViewPosition(x, y);
}

bool pdcpp::ListBox::isItemVisible(int itemIndex)
{
    auto itemBounds = m_Items[itemIndex]->getBounds();

    const auto viewBounds = m_ItemView.getBounds();
    const auto viewOffset = m_ItemView.getContentOffset();

    itemBounds = {itemBounds.x + viewOffset.x + viewBounds.x, itemBounds.y + viewBounds.y + viewOffset.y, itemBounds.width, itemBounds.height};

    const auto overlap = pdcpp::RectHelpers::getOverlappingRect(itemBounds, viewBounds);
    return overlap.width == itemBounds.width && overlap.height == itemBounds.height;
}


pdcpp::ListBox::ItemProperty::ItemProperty(pdcpp::ListBox& owner, bool multiMark)
    : r_Owner(owner)
    , m_MultiMark(multiMark)
{}

void pdcpp::ListBox::ItemProperty::markItem(int item, bool bringIntoView, bool unmarkOthers)
{
    if (!m_MultiMark)
        { unmarkOthers = true; }

    if (isItemMarked(item) && !(unmarkOthers && getNumMarked() > 1))
        { return; }

    const auto bounds = r_Owner.getBounds();
    if (bounds.height == 0 || bounds.width == 0)
        { bringIntoView = true; }

    if (unmarkOthers)
        { m_Marked.clear(); }

    m_Marked.insert(item);
    r_Owner.updateContent();

    if (bringIntoView)
        { r_Owner.bringItemIntoView(item); }
}

bool pdcpp::ListBox::ItemProperty::isItemMarked(int itemIndex) const
    { return std::any_of(m_Marked.begin(), m_Marked.end(), [itemIndex](auto x) { return x == itemIndex; } ); }

size_t pdcpp::ListBox::ItemProperty::getNumMarked() const
    { return m_Marked.size(); }

void pdcpp::ListBox::ItemProperty::unmarkItem(int itemIndex)
{
    m_Marked.erase(itemIndex);
    r_Owner.updateContent();
}

int pdcpp::ListBox::ItemProperty::getMarkedItem(int itemIndex) const
{
    return m_Marked[itemIndex];
}
