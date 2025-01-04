//
// Created by Matt on 12/24/2024.
//

#include "pdcpp/components/ListMenuComponent.h"
#include "pdcpp/graphics/LookAndFeel.h"

class IconComponent
    : public pdcpp::Component
{
public:
    explicit IconComponent(const pdcpp::MenuComponentBase::Icon& icon)
        : m_Icon(icon)
        , m_Text("")
        , m_Focused(false)
    {
        std::visit(pdcpp::Overload{
            [&](const std::string& value) { m_Text.setText(value); },
            [](pdcpp::Component*){}
        }, icon);
    };

    void setFocus(bool isFocused)
    {
        m_Focused = isFocused;
        m_Text.setColor(
            pdcpp::TextComponent::backgroundColorId,
            isFocused ? pdcpp::Colors::diagonalLinesRightWhiteOnBlack : pdcpp::Colors::white
        );
        m_Text.setColor(
            pdcpp::TextComponent::outlineColorId,
            isFocused ? pdcpp::Colors::diagonalLinesRightWhiteOnBlack : pdcpp::Colors::white
        );
        m_Text.setColor(
            pdcpp::TextComponent::textColorId,
            isFocused ? pdcpp::Colors::white : pdcpp::Colors::black
        );
    }

protected:
    void draw() override
    {
        auto bounds = getBounds();

        pdcpp::Graphics::fillRectangle(
            bounds.toInt(), m_Focused ? pdcpp::Colors::diagonalLinesRightWhiteOnBlack : pdcpp::Colors::white);

        std::visit(pdcpp::Overload{
            [&](const std::string& value)
            {
                m_Text.setBounds(bounds);
                m_Text.redraw();
            },
            [&](pdcpp::Component* value)
            {
                value->setBounds(value->getBounds().withCenter(bounds.getCenter()));
                value->redraw();
            }
        }, m_Icon);

    }

private:
    pdcpp::TextComponent m_Text;
    const pdcpp::MenuComponentBase::Icon& m_Icon;
    bool m_Focused;
};

pdcpp::ListMenuComponent::ListMenuComponent
(std::vector<MenuItem> menu, std::function<void()> nonAction, bool horizontal)
    : MenuComponentBase(std::move(menu), std::move(nonAction))
    , m_Horiz(horizontal)
{ setCellFocus(-1, -1, false, false); }

int pdcpp::ListMenuComponent::getNumRows() const { return m_Horiz ? 1 : getMenuItems().size(); }

int pdcpp::ListMenuComponent::getNumCols() const { return m_Horiz ? getMenuItems().size() : 1; }

int pdcpp::ListMenuComponent::getRowHeight(int i) const
{
    if (!m_Horiz)
        { return getMenuItems()[i].getBounds(getLookAndFeel()->getDefaultFont()).height; }

    float height = getBounds().height;

    for (auto& item : getMenuItems())
    {
        auto itemBounds = item.getBounds(getLookAndFeel()->getDefaultFont());
        height = std::max(itemBounds.height, height);
    }

    return height;
}

int pdcpp::ListMenuComponent::getColWidth(int i) const
{
    if (m_Horiz)
        { return getMenuItems()[i].getBounds(getLookAndFeel()->getDefaultFont()).width; }

    float width = getBounds().width;

    for (auto& item : getMenuItems())
    {
        auto itemBounds = item.getBounds(getLookAndFeel()->getDefaultFont());
        width = std::max(itemBounds.width, width);
    }

    return width;
}

pdcpp::Component* pdcpp::ListMenuComponent::refreshComponentForCell
(int row, int column, bool hasFocus, pdcpp::Component* toUpdate)
{
    std::unique_ptr<IconComponent> icon(dynamic_cast<IconComponent*>(toUpdate));
    if (icon == nullptr)
        { icon = std::make_unique<IconComponent>(getMenuItems()[m_Horiz ? column : row].icon); }

    icon->setFocus(hasFocus);
    return icon.release();
}

void pdcpp::ListMenuComponent::selectedChanged()
{
    if (getSelectedIndex() < 0) { return; }
    setCellFocus(m_Horiz ? 0 : getSelectedIndex(), m_Horiz ? getSelectedIndex() : 0);
}

