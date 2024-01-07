//
// Created by Matt on 11/8/2023.
//

#include "App.h"
#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/graphics/Colors.h>
#include <pdcpp/graphics/ScopedGraphicsContext.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

const std::string kLorumIpsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore "
                                "magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
                                "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
                                "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";


TextBlock::TextBlock(std::string text, const pdcpp::Font& font)
    : m_Text(std::move(text))
    , r_Font(font)
{}

void TextBlock::draw()
{
    auto bounds = getLocalBounds();
    bounds.height = r_Font.drawWrappedText(m_Text, bounds);
    setBounds(bounds);
}

ABMenu::ABMenu(std::string text)
    : m_Text(std::move(text), pdcpp::LookAndFeel::getDefaultLookAndFeel()->getDefaultFont())
    , m_Viewport(&m_Text)
{
    setVisible(false);
}

void ABMenu::redraw(const pdcpp::Rectangle<float>& bounds, const pdcpp::Rectangle<float>& drawrect)
{
    pdcpp::Graphics::fillRoundedRectangle(bounds.reduced(1).toInt(), 10, pdcpp::Colors::solid50GrayA);
    pdcpp::Graphics::drawRoundedRectangle(bounds.reduced(1).toInt(), 10, 2);
    pdcpp::Graphics::fillRoundedRectangle(bounds.reduced(5).toInt(), 10, pdcpp::Colors::steppedDither6);

    auto innerBounds = bounds.reduced(15);
    innerBounds.removeFromLeft(10); innerBounds.removeFromRight(10); // Trim the margins
    auto buttonBounds = innerBounds.removeFromBottom(50);

    pdcpp::Graphics::fillRoundedRectangle(innerBounds.reduced(1).toInt(), 10, pdcpp::Colors::solid50GrayA);
    pdcpp::Graphics::fillRoundedRectangle(innerBounds.reduced(5).toInt(), 5, kColorWhite);
    pdcpp::Graphics::drawRoundedRectangle(innerBounds.reduced(5).toInt(), 5, 2);

    m_Viewport.draw();
}

void ABMenu::resized()
{
    auto bounds = getBounds().reduced(15);
    bounds.removeFromLeft(10);
    bounds.removeFromRight(10);
    bounds.removeFromBottom(50);
    bounds = bounds.reduced(10);

    m_Text.setBounds({0, 0, bounds.width, 500});
    m_Viewport.setBounds(bounds);
}

void ABMenu::crankStateChanged(float absolute, float delta)
{
    m_Viewport.moveContentBy(0, -delta, true);
}

void ABMenu::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{
    if (PDButtons::kButtonB & pressed)
    {
        popContext();
    }
}

void ABMenu::contextEntered(){ setVisible(true); }
void ABMenu::contextExited() { setVisible(false); }

App::App()
    : m_ContextManager(this)
    , m_Menu(kLorumIpsum)
{
    m_Menu.setBounds(pdcpp::Graphics::getScreenBounds().reduced(20));
}

int App::update()
{
    m_ContextManager.update();
    pdcpp::Sprite::updateAndRedrawAllSprites();
    if (m_ContextManager.getCurrentContext() == this)
    {
        auto screenBounds = pdcpp::Graphics::getScreenBounds();
        pdcpp::ScopedGraphicsContext context(screenBounds);

        std::string msg = "Press A to change contexts.";
        const auto font = pdcpp::LookAndFeel::getDefaultLookAndFeel()->getDefaultFont();
        const auto screenCenter = screenBounds.getCenter();
        const auto point = pdcpp::Rectangle<float>(0, 0, font.getTextWidth(msg), font.getFontHeight()).withCenter(screenCenter).getTopLeft();
        font.drawText(msg, point.x, point.y);
    }
    return 1;
}

void App::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{
    if (PDButtons::kButtonA & pressed)
        { pushChildContext(&m_Menu); }
}
