//
// Created by Matt on 12/15/2024.
//

#pragma once
#include <variant>
#include <functional>
#include <pdcpp/components/Component.h>
#include <pdcpp/graphics/Font.h>


namespace pdcpp
{
    class MenuComponentBase
    {
    public:
        using Icon = std::variant<std::string, pdcpp::Component*>;

        struct MenuItem
        {
            pdcpp::MenuComponentBase::Icon icon;
            std::function<void()> action;

           [[ nodiscard ]] pdcpp::Rectangle<float> getBounds(pdcpp::Font& font) const;
        };

        explicit MenuComponentBase(
            std::vector <MenuItem> menu, std::function<void()> nonAction = []()
        {});

        void setSelectedIndex(int i);

        [[nodiscard]] int getSelectedIndex() const { return m_Selected; }
        [[nodiscard]] const std::vector <MenuItem>& getMenuItems() const { return m_Menu; }

        void executeSelectedAction() const;

    protected:
        virtual void selectedChanged() = 0;

    private:
        std::vector<MenuItem> m_Menu;
        std::function<void()> m_AbortAction;
        int m_Selected;
    };
}