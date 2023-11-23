//
// Created by Matt on 11/19/2023.
//
#include <algorithm>
#include <cassert>
#include "pdcpp/components/Component.h"


void pdcpp::Component::setBounds(PDRect bounds) { m_Bounds = bounds; resized(bounds); }
PDRect pdcpp::Component::getBounds() const { return m_Bounds; }

void pdcpp::Component::redraw()
{
    draw();
    for (auto child : m_Children)
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
    m_Children.erase(std::remove_if(m_Children.begin(), m_Children.end(), [child](auto x) { return x == child; }));
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
