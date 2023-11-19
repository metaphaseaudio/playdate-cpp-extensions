//
// Created by Matt on 11/19/2023.
//
#include "pdcpp/components/Component.h"


void pdcpp::Component::setBounds(PDRect bounds) { m_Bounds = bounds; resized(bounds); }
PDRect pdcpp::Component::getBounds() const { return m_Bounds; }

void pdcpp::Component::redraw()
{
    draw();
    for (auto child : m_Children) { child->redraw(); }
}

void pdcpp::Component::addChild(pdcpp::Component* child)

{
    m_Children.emplace_back(child);
}

void pdcpp::Component::removeChild(pdcpp::Component* child)
{
    m_Children.erase(std::remove_if(m_Children.begin(), m_Children.end(), [child](auto x) { return x == child; }));
}
