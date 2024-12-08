/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 11/19/2023
 *  Original author: MrBZapp
 */

#include <algorithm>
#include <cassert>
#include "pdcpp/components/Component.h"
#include "pdcpp/graphics/LookAndFeel.h"


void pdcpp::Component::setBounds(pdcpp::Rectangle<float> bounds)
{
    m_Bounds = bounds;
    resized(bounds);
}

pdcpp::Rectangle<float> pdcpp::Component::getBounds() const { return m_Bounds; }

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

    // Don't add components to more than one parent
    assert(child->p_Parent == nullptr);

    m_Children.emplace_back(child);
    child->p_Parent = this;
    child->lookAndFeelChanged();
}

void pdcpp::Component::removeChildComponent(Component* child)
{
    auto itr = std::remove_if(m_Children.begin(), m_Children.end(), [child](auto x) { return x == child; });
    (*itr)->p_Parent = nullptr;
    (*itr)->lookAndFeelChanged();
    m_Children.erase(itr, m_Children.end());
}

size_t pdcpp::Component::childCount() const { return m_Children.size(); }

pdcpp::Component* pdcpp::Component::getChildComponent(int index) const { return m_Children[index]; }

const std::vector<pdcpp::Component*>& pdcpp::Component::getChildren() const { return m_Children; }

pdcpp::Component* pdcpp::Component::getParentComponent() const { return p_Parent; }

void pdcpp::Component::removeAllChildren()
{
    for (auto c : m_Children)
    {
        c->p_Parent = nullptr;
        c->lookAndFeelChanged();
    }

    m_Children.clear();
}

void pdcpp::Component::resizeToFitChildren()
{
    if (m_Children.empty())
    {
        setBounds({0, 0, 0, 0 });
        return;
    }

    pdcpp::Rectangle<float> bounds = m_Children[0]->getBounds();
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

pdcpp::Rectangle<float> pdcpp::Component::getLocalBounds() const
{
    const auto bounds = getBounds();
    return {0, 0, bounds.width, bounds.height};
}

pdcpp::LookAndFeel* pdcpp::Component::getLookAndFeel() const
{
    if (m_CustomLookAndFeel != nullptr)
        { return m_CustomLookAndFeel; }

    if (p_Parent)
        { return p_Parent->getLookAndFeel(); }

    return pdcpp::LookAndFeel::getDefaultLookAndFeel();
}

void pdcpp::Component::setLookAndFeel(pdcpp::LookAndFeel* newLAF)
{
    m_CustomLookAndFeel = newLAF;

    for (auto& child : m_Children)
        { child->setLookAndFeel(newLAF); }

    lookAndFeelChanged();
}

pdcpp::Color pdcpp::Component::findColor(int colourID, bool inheritFromParent) const
{
    if (m_Colors.contains(colourID))
        { return m_Colors.at(colourID); }

    if (inheritFromParent && p_Parent != nullptr)
        { return p_Parent->findColor(colourID, true); }

    return getLookAndFeel()->findColor(colourID);
}

void pdcpp::Component::setColor(int colorID,  pdcpp::Color color) { m_Colors[colorID] = color; }
void pdcpp::Component::resetColorToDefault(int colorID){ if (m_Colors.contains(colorID)) { m_Colors.erase(colorID); } }
bool pdcpp::Component::isColorSpecified(int colorID) const { return m_Colors.contains(colorID); }
