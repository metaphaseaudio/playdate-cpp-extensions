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
    , m_Viewport(m_FocusContainer.get())
{ addChildComponent(&m_Viewport); }


void pdcpp::ComponentFocusView::setComponentFocus(size_t index)
{
    if (index < 0 || index >= m_FocusContainer->childCount()) { return; }
    const auto bounds = getBounds();
    const auto focusBounds = m_FocusContainer->getChildComponent(m_FocusIndex)->getBounds();
    const pdcpp::Point<float> focusCenter = pdcpp::RectHelpers::getCenter(focusBounds);
    const pdcpp::Point<float> boundsCenter = pdcpp::RectHelpers::getCenter({0, 0, bounds.width, bounds.height});
    pdcpp::Point<float> target = {boundsCenter.x - focusCenter.x, boundsCenter.y - focusCenter.y};
    m_Viewport.setContentOffset(target.x, target.y);
    m_FocusIndex = index;
}

int pdcpp::ComponentFocusView::getComponentFocusIndex() const { return m_FocusIndex; }

void pdcpp::ComponentFocusView::resized(PDRect newBounds)
    { m_Viewport.setBounds(newBounds); }

void pdcpp::ComponentFocusView::addChildToFocusContainer(pdcpp::Component* child)
{
    m_FocusContainer->addChildComponent(child);
    m_FocusContainer->resizeToFitChildren();
    if (m_FocusIndex < 0)
        { setComponentFocus(0); }
}

void pdcpp::ComponentFocusView::removeChildFromFocusContainer(pdcpp::Component* child, bool resetFocus)
{
    m_FocusContainer->removeChildComponent(child);
    m_FocusIndex = std::min(m_FocusIndex, int(m_FocusContainer->childCount()) - 1);
    m_FocusContainer->resizeToFitChildren();

    if (resetFocus)
        { setComponentFocus(m_FocusIndex); }
}



void pdcpp::ComponentFocusView::clearFocusView()
{
    m_FocusContainer->removeAllChildren();
}
