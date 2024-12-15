//
// Created by Matt on 12/15/2024.
//

#include "pdcpp/components/MenuComponentBase.h"

pdcpp::MenuComponentBase::MenuComponentBase(std::vector<MenuItem> menu, std::function<void()> nonAction)
        : m_Menu(std::move(menu))
        , m_AbortAction(std::move(nonAction))
        , m_Selected(-1)
{}

void pdcpp::MenuComponentBase::resized(const pdcpp::Rectangle<float>& newBounds)
{
    m_PreRenderedImage = buildPreRenderedImage();
}

void pdcpp::MenuComponentBase::draw()
{
    m_PreRenderedImage.draw(getBounds().getTopLeft().toInt());
}

void pdcpp::MenuComponentBase::setSelectedIndex(int i)
{
    m_Selected = i;
    m_PreRenderedImage = buildPreRenderedImage();
}

void pdcpp::MenuComponentBase::executeSelectedAction() const
{
    if (m_Selected < 0 || m_Selected >= m_Menu.size())
    {
        m_AbortAction();
        return;
    }
    m_Menu[m_Selected].action();
}
