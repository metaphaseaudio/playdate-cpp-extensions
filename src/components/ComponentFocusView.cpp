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


void pdcpp::ComponentFocusView::setComponentFocus(size_t index, int transitionFrames)
{
    if (index < 0 || index >= m_FocusContainer->childCount()) { return; }

    m_FocusIndex = index;
    m_TransitionFramesRemaining = transitionFrames;
    if (transitionFrames == 0)
        { updateAnimation(); }
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

bool pdcpp::ComponentFocusView::updateAnimation()
{
    if (m_TransitionFramesRemaining >= 0)
    {
        const auto bounds = getBounds();
        const auto focusBounds = m_FocusContainer->getChildComponent(m_FocusIndex)->getBounds();
        const pdcpp::Point<float> focusCenter = pdcpp::RectHelpers::getCenter(focusBounds);
        const pdcpp::Point<float> boundsCenter = pdcpp::RectHelpers::getCenter({0, 0, bounds.width, bounds.height});
        pdcpp::Point<float> target = {boundsCenter.getX() - focusCenter.getX(), boundsCenter.getY() - focusCenter.getY()};
        if (m_TransitionFramesRemaining != 0)
        {
            auto current = m_Viewport.getContentOffset();
            target = {
                ((target.getX() - current.getX()) / m_TransitionFramesRemaining) + current.getX(),
                ((target.getY() - current.getY()) / m_TransitionFramesRemaining) + current.getY()
            };
        }

        m_Viewport.setContentOffset(target.getX(), target.getY());
        m_TransitionFramesRemaining--;
        return true;
    }
    return false;
}

void pdcpp::ComponentFocusView::clearFocusView()
{
    m_FocusContainer->removeAllChildren();
}
