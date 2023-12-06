//
// Created by Matt on 11/20/2023.
//

#include "pdcpp/components/ComponentFocusView.h"
#include "pdcpp/graphics/RectHelpers.h"

class pdcpp::ComponentFocusView::FocusContainer
    : public pdcpp::Component
{};


pdcpp::ComponentFocusView::ComponentFocusView()
    : m_FocusContainer(std::make_unique<FocusContainer>())
//    , m_Viewport(m_FocusContainer.get())
{ addChildComponent(&m_Viewport); }


void pdcpp::ComponentFocusView::setComponentFocus(size_t index)
{
    if (index < 0 || index >= m_FocusContainer->childCount()) { return; }
    const auto bounds = getBounds();
    const auto focusBounds = m_FocusContainer->getChildComponent(index)->getBounds();
    const pdcpp::Point<float> focusCenter = pdcpp::RectHelpers::getCenter(focusBounds);
    const pdcpp::Point<float> boundsCenter = pdcpp::RectHelpers::getCenter({0, 0, bounds.width, bounds.height});
    pdcpp::Point<float> target = {boundsCenter.x - focusCenter.x, boundsCenter.y - focusCenter.y};
    m_Viewport.setContentOffset(target.x, target.y);
}


void pdcpp::ComponentFocusView::resized(const pdcpp::Rectangle<float>& newBounds)
    { m_Viewport.setBounds(newBounds); }

void pdcpp::ComponentFocusView::addChildToFocusContainer(pdcpp::Component* child)
{
    m_FocusContainer->addChildComponent(child);
    m_FocusContainer->resizeToFitChildren();
}

void pdcpp::ComponentFocusView::removeChildFromFocusContainer(pdcpp::Component* child, bool resetFocus)
{
    m_FocusContainer->removeChildComponent(child);
    m_FocusContainer->resizeToFitChildren();
}


void pdcpp::ComponentFocusView::clearFocusView()
{
    m_FocusContainer->removeAllChildren();
}

void pdcpp::ComponentFocusView::bringComponentIntoView(size_t index)
{
    if (isComponentVisible(index)) { return; }

    auto itemBounds = m_FocusContainer->getChildren()[index]->getBounds();
    const auto viewBounds = m_Viewport.getBounds();
    const auto viewPosition = m_Viewport.getViewPosition();
    itemBounds = {itemBounds.x - viewPosition.x, itemBounds.y - viewPosition.y, itemBounds.width, itemBounds.height};

    const pdcpp::Point<float> viewBottomRight = pdcpp::RectHelpers::getBottomRight(viewBounds);
    const pdcpp::Point<float> itemBottomRight = pdcpp::RectHelpers::getBottomRight(itemBounds);
    int x = viewPosition.x, y = viewPosition.y;

    if      (itemBounds.x > viewBottomRight.x) { x = viewPosition.x + (itemBottomRight.x - viewBottomRight.x); } // Move left
    else if (viewBounds.x > itemBottomRight.x) { x = itemBounds.x; } // Move right

    if      (itemBounds.y > viewBottomRight.y) { y = viewPosition.y + (itemBottomRight.y - viewBottomRight.y); } // Move up
    else if (viewBounds.y > itemBottomRight.y) { y = itemBounds.y; } // Move down

    m_Viewport.setViewPosition(x, y);
}

bool pdcpp::ComponentFocusView::isComponentVisible(size_t index) const
{
    auto itemBounds = m_FocusContainer->getChildren()[index]->getBounds();
    const auto viewBounds = m_Viewport.getBounds();
    const auto viewOffset = m_Viewport.getContentOffset();

    itemBounds = {itemBounds.x - viewOffset.x, itemBounds.y - viewOffset.y, itemBounds.width, itemBounds.height};

    const auto overlap = pdcpp::RectHelpers::getOverlappingRect(itemBounds, viewBounds);
    return overlap.width == itemBounds.width && overlap.height == itemBounds.height;
}
