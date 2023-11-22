//
// Created by Matt on 11/20/2023.
//

#include <cassert>
#include "pdcpp/components/ComponentFocusView.h"

class pdcpp::ComponentFocusView::FocusContainer
    : public pdcpp::Component
{};


pdcpp::ComponentFocusView::ComponentFocusView()
    : m_FocusContainer(std::make_unique<FocusContainer>())
    , m_Viewport(m_FocusContainer.get())
{
    addChildComponent(&m_Viewport);
}


void pdcpp::ComponentFocusView::setComponentFocus(size_t index, int transitionFrames)
{
    if (index < 0 || index >= m_FocusContainer->childCount()) { return; }

    m_FocusIndex = index;
    m_TransitionFramesRemaining = transitionFrames;
    if (transitionFrames == 0)
        { updateTransition(); }
}

int pdcpp::ComponentFocusView::getComponentFocusIndex() const
{
    return m_FocusIndex;
}

void pdcpp::ComponentFocusView::resized(PDRect newBounds)
    { m_Viewport.setBounds(newBounds); }

void pdcpp::ComponentFocusView::addChildToFocusContainer(pdcpp::Component* child)
{
    m_FocusContainer->addChildComponent(child);
    resizeFocusContainer();
    if (m_FocusIndex < 0)
        { setComponentFocus(0); }
}

void pdcpp::ComponentFocusView::removeChildFromFocusContainer(pdcpp::Component* child, bool resetFocus)
{
    m_FocusContainer->removeChildComponent(child);
    m_FocusIndex = std::min(m_FocusIndex, int(m_FocusContainer->childCount()) - 1);
    resizeFocusContainer();

    if (resetFocus)
        { setComponentFocus(m_FocusIndex); }
}

void pdcpp::ComponentFocusView::resizeFocusContainer()
{
    PDRect bounds = m_FocusContainer->getChildComponent(0)->getBounds();
    for (auto child : m_FocusContainer->getChildren())
    {
        const auto childBounds = child->getBounds();
        bounds.x = std::min(bounds.x, childBounds.x);
        bounds.y = std::min(bounds.y, childBounds.y);
        bounds.width = std::max(bounds.width, (childBounds.width + childBounds.x) - bounds.x);
        bounds.height = std::max(bounds.height, (childBounds.height + childBounds.y) - bounds.y);
    }
    m_FocusContainer->setBounds(bounds);
}

void pdcpp::ComponentFocusView::updateTransition()
{
    if (m_TransitionFramesRemaining >= 0)
    {
        auto focusBounds = m_FocusContainer->getChildComponent(m_FocusIndex)->getBounds();
        pdcpp::Point<float> target = {-focusBounds.x + (focusBounds.width * 0.5f), -focusBounds.y + (focusBounds.height * 0.5f)};

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
    }
    redraw();
}
