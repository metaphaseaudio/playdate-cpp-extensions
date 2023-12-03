//
// Created by Matt on 11/19/2023.
//
#include <algorithm>
#include <cassert>
#include "pdcpp/components/Component.h"
#include "pdcpp/graphics/ScopedGraphicsContext.h"


void pdcpp::Component::setBounds(PDRect bounds)
{
    m_Bounds = bounds;
    resized(bounds);
}

PDRect pdcpp::Component::getBounds() const { return m_Bounds; }

void pdcpp::Component::redraw()
{
    draw();
    for (auto* child : m_Children)
        { child->redraw(); }
}

void pdcpp::Component::addChildComponent(Component* child)
{
    // Don't add yourself, that's a recipe for a stack overflow.
    assert(child != this);
    m_Children.emplace_back(child);
}

void pdcpp::Component::removeChildComponent(Component* child)
{
    m_Children.erase(std::remove_if(m_Children.begin(), m_Children.end(), [child](auto x) { return x == child; }), m_Children.end());
}

size_t pdcpp::Component::childCount() const
{
    return m_Children.size();
}

pdcpp::Component* pdcpp::Component::getChildComponent(int index) const
{
    return m_Children[index];
}

const std::vector<pdcpp::Component*>& pdcpp::Component::getChildren() const { return m_Children; }

void pdcpp::Component::removeAllChildren() { m_Children.clear(); }

void pdcpp::Component::resizeToFitChildren()
{
    if (m_Children.empty())
    {
        setBounds({0, 0, 0, 0 });
        return;
    }

    PDRect bounds = m_Children[0]->getBounds();
    for (auto child : m_Children)
    {
        const auto childBounds = child->getBounds();
        bounds.x = std::min(bounds.x, childBounds.x);
        bounds.y = std::min(bounds.y, childBounds.y);
        bounds.width = std::max(bounds.width, (childBounds.width + childBounds.x) - bounds.x);
        bounds.height = std::max(bounds.height, (childBounds.height + childBounds.y) - bounds.y);
    }

    setBounds(bounds);
}

PDRect pdcpp::Component::getLocalBounds() const
{
    const auto bounds = getBounds();
    return {0, 0, bounds.width, bounds.height};
}
