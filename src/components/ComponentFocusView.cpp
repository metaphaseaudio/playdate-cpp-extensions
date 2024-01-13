/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 11/20/2023
 *  Original author: MrBZapp
 */

#include "pdcpp/components/ComponentFocusView.h"
#include "pdcpp/graphics/RectHelpers.h"

class pdcpp::ComponentFocusView::FocusContainer
    : public pdcpp::Component
{};


pdcpp::ComponentFocusView::ComponentFocusView()
    : m_FocusContainer(std::make_unique<FocusContainer>())
{ setContent(m_FocusContainer.get()); }


void pdcpp::ComponentFocusView::setComponentFocus(size_t index)
{
    if (index < 0 || index >= m_FocusContainer->childCount()) { return; }
    const auto bounds = getBounds();
    const auto focusBounds = m_FocusContainer->getChildComponent(index)->getBounds();
    const pdcpp::Point<float> focusCenter = pdcpp::RectHelpers::getCenter(focusBounds);
    const pdcpp::Point<float> boundsCenter = pdcpp::RectHelpers::getCenter({0, 0, bounds.width, bounds.height});
    pdcpp::Point<float> target = {boundsCenter.x - focusCenter.x, boundsCenter.y - focusCenter.y};
    setContentOffset(target.x, target.y);
}

void pdcpp::ComponentFocusView::addChildToFocusContainer(pdcpp::Component* child)
{
    m_FocusContainer->addChildComponent(child);
    m_FocusContainer->resizeToFitChildren();
}

void pdcpp::ComponentFocusView::removeChildFromFocusContainer(pdcpp::Component* child)
{
    m_FocusContainer->removeChildComponent(child);
    m_FocusContainer->resizeToFitChildren();
}


void pdcpp::ComponentFocusView::clearFocusView() { m_FocusContainer->removeAllChildren(); }

void pdcpp::ComponentFocusView::bringComponentIntoView(size_t index)
{
    if (isComponentVisible(index)) { return; }

    auto itemBounds = m_FocusContainer->getChildren()[index]->getBounds();
    const auto viewBounds = getBounds();
    const auto viewPosition = getViewPosition();
    const auto viewOffset = getContentOffset();
    itemBounds = {itemBounds.x + viewBounds.x + viewOffset.x, itemBounds.y + viewBounds.y + viewOffset.y, itemBounds.width, itemBounds.height};

    const pdcpp::Point<float> viewBottomRight = pdcpp::RectHelpers::getBottomRight(viewBounds);
    const pdcpp::Point<float> itemBottomRight = pdcpp::RectHelpers::getBottomRight(itemBounds);
    int x = viewPosition.x, y = viewPosition.y;

    if      (itemBottomRight.x > viewBottomRight.x) { x = viewPosition.x + (itemBottomRight.x - viewBottomRight.x); } // Move left
    else if (viewBounds.x > itemBounds.x)           { x = viewPosition.x + (itemBounds.x - viewBounds.x); } // Move right
    if      (itemBottomRight.y > viewBottomRight.y) { y = viewPosition.y + (itemBottomRight.y - viewBottomRight.y); } // Move up
    else if (viewBounds.y > itemBounds.y)           { y = viewPosition.y + (itemBounds.y - viewBounds.y); } // Move down

    setViewPosition(x, y);
}

bool pdcpp::ComponentFocusView::isComponentVisible(size_t index) const
{
    auto itemBounds = m_FocusContainer->getChildren()[index]->getBounds();
    const auto viewBounds = getBounds();
    const auto viewOffset = getContentOffset();

    itemBounds = {itemBounds.x + viewBounds.x + viewOffset.x, itemBounds.y + viewBounds.y + viewOffset.y, itemBounds.width, itemBounds.height};

    const auto overlap = pdcpp::RectHelpers::getOverlappingRect(itemBounds, viewBounds);
    return overlap.width == itemBounds.width && overlap.height == itemBounds.height;
}

pdcpp::Component* pdcpp::ComponentFocusView::getFocusContainerChild(int i) const { return m_FocusContainer->getChildComponent(i); }
