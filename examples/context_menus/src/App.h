//
// Created by Matt on 11/8/2023.
//

#pragma once
#include <pdcpp/graphics/Sprite.h>
#include <pdcpp/graphics/Font.h>
#include <pdcpp/components/Viewport.h>
#include <pdcpp/core/InputContext.h>

class TextBlock
    : public pdcpp::Component
{
public:
    TextBlock(std::string text, const pdcpp::Font& font);

protected:
    void draw() override;
private:
    std::string m_Text;
    const pdcpp::Font& r_Font;
};


class ABMenu
    : public pdcpp::Sprite
    , public pdcpp::InputContext
{
public:
    explicit ABMenu(std::string text);

    void contextEntered() override;

    void contextExited() override;

    void redraw(const pdcpp::Rectangle<float>& bounds, const pdcpp::Rectangle<float>& drawrect) override;

protected:
    void crankStateChanged(float absolute, float delta) override;

    void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) override;

public:

    void resized() override;

private:
    TextBlock m_Text;
    pdcpp::Viewport m_Viewport;

};

class App
    : public pdcpp::InputContext
{
public:
    App();
    int update();

protected:
    void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) override;

private:
    ABMenu m_Menu;
    pdcpp::InputContextManager m_ContextManager;
};
