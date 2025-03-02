//
// Created by Matt on 12/15/2024.
//

#include "pdcpp/components/MenuComponentBase.h"
#include "pdcpp/core/util.h"

pdcpp::MenuComponentBase::MenuComponentBase(std::vector<MenuItem> menu, std::function<void()> nonAction)
    : m_Menu(std::move(menu))
    , m_AbortAction(std::move(nonAction))
    , m_Selected(-1)
{}

void pdcpp::MenuComponentBase::setSelectedIndex(int i)
{
    m_Selected = i;
    selectedChanged();
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

pdcpp::Rectangle<float> pdcpp::MenuComponentBase::MenuItem::getBounds(pdcpp::Font& font) const
{
    return std::visit(
        Overload
        {
            [&](const std::string& str) { return font.getTextArea(str).toFloat(); },
            [](Component* component) { return component->getBounds(); }
        }, icon
    );
}

