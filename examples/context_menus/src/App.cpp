/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2024 - Metaphase
 *
 *  Created: 1/06/2024
 *  Original author: MrBZapp
 */
#include "App.h"
#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/graphics/Colors.h>
#include <pdcpp/graphics/ScopedGraphicsContext.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>
#include <pdcpp/components/RingMenu.h>

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
    auto targetHeight = r_Font.drawWrappedText(m_Text, bounds);
    bounds.height = targetHeight;
    setBounds(bounds);
}


void ExitButton::draw()
{
    pdcpp::ScopedGraphicsContext context(getBounds());
    pdcpp::GlobalPlaydateAPI::get()->graphics->setDrawMode(m_IsPressed ? kDrawModeCopy : kDrawModeNXOR);
    pdcpp::Graphics::fillRoundedRectangle(getLocalBounds().toInt().reduced(1), 10, m_IsPressed ? kColorWhite : pdcpp::Colors::solid50GrayA);
    pdcpp::Graphics::drawRoundedRectangle(getLocalBounds().toInt().reduced(1), 10, 2);
    pdcpp::Graphics::fillRoundedRectangle(getLocalBounds().toInt().reduced(5), 10, m_IsPressed ? pdcpp::Colors::solid50GrayA : kColorWhite);

    std::string msg = "Press  B  to exit context.";
    const auto font = getLookAndFeel()->getDefaultFont();
    const auto point = getLocalBounds().toInt().getTopLeft() + pdcpp::Point<int>(14, 14);
    auto circleBounds = pdcpp::Rectangle<int>(point.x + font.getTextWidth("Press ") - 1, point.y - 2, 20, 20);
    pdcpp::Graphics::fillEllipse(circleBounds, 0, 0,  m_IsPressed ? kColorWhite : kColorBlack);
    font.drawText(msg, point.x, point.y);
}

CrankContext::CrankContext(std::string text)
    : m_Text(std::move(text), pdcpp::LookAndFeel::getDefaultLookAndFeel()->getDefaultFont())
    , m_Viewport(&m_Text)
{ setVisible(false); }


void CrankContext::redraw(const pdcpp::Rectangle<float>& bounds, const pdcpp::Rectangle<float>& drawrect)
{
    // Draw the "body"
    pdcpp::Graphics::fillRoundedRectangle(bounds.reduced(1).toInt(), 10, pdcpp::Colors::solid50GrayA);
    pdcpp::Graphics::drawRoundedRectangle(bounds.reduced(1).toInt(), 10, 2);
    pdcpp::Graphics::fillRoundedRectangle(bounds.reduced(5).toInt(), 10, pdcpp::Colors::steppedDither4);

    auto innerBounds = bounds.reduced(15);

    // make room to draw the button
    innerBounds.removeFromBottom(50);

    // Draw the "screen"
    innerBounds.removeFromLeft(10); innerBounds.removeFromRight(10); // Trim the margins
    pdcpp::Graphics::fillRoundedRectangle(innerBounds.reduced(1).toInt(), 10, pdcpp::Colors::solid50GrayA);
    pdcpp::Graphics::fillRoundedRectangle(innerBounds.reduced(5).toInt(), 5, pdcpp::Colors::steppedDither6);
    pdcpp::Graphics::drawRoundedRectangle(innerBounds.reduced(5).toInt(), 5, 2);

    m_Viewport.redraw();
    m_Button.redraw();
}

void CrankContext::resized()
{
    // We basically have to set everything up here w/r/t position/shape
    auto bounds = getBounds().reduced(15);
    bounds.removeFromLeft(10);
    bounds.removeFromRight(10);

    auto buttonBounds = bounds.removeFromBottom(50);
    buttonBounds.removeFromTop(5);
    buttonBounds.removeFromLeft(15);
    buttonBounds.removeFromRight(15);
    bounds = bounds.reduced(10);

    // Setting everything except the width to 0 here, the text will set its own
    // height when rendered. This is a *biiit* awkward because that size reset
    // won't refresh the drawing, so it'll show up blank the first time without
    // a deliberate "initial" re-draw. This is done on context entry later on.
    m_Text.setBounds({0, 0, bounds.width, 0});

    m_Button.setBounds(buttonBounds);
    m_Viewport.setBounds(bounds);
}

void CrankContext::crankStateChanged(float absolute, float delta) { m_Viewport.moveContentBy(0, -delta, true); }

void CrankContext::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{
    if (PDButtons::kButtonB & pressed)
    {
        m_Button.pressed(true);
    }
    if (PDButtons::kButtonB & released)
    {
        m_Button.pressed(false);
        popContext();
    }
}

void CrankContext::contextEntered(){ setVisible(true); m_Viewport.redraw(); }
void CrankContext::contextExited() { setVisible(false); }

App::App()
    : m_ContextManager(this)
    , m_Menu(kLorumIpsum)
    , m_RingMenu({"Edit\nObject", "Tools", "Synth", "World"}, -45)
{
    auto screenBounds = pdcpp::Graphics::getScreenBounds();
    m_Menu.setBounds(screenBounds.reduced(20).toFloat());
    m_RingMenu.setBounds(pdcpp::Rectangle<int>(0, 0, screenBounds.height, screenBounds.height).withCenter(screenBounds.getCenter()).toFloat());
}

int App::update()
{
    m_ContextManager.update();
    pdcpp::Sprite::updateAndRedrawAllSprites();
    if (m_ContextManager.getCurrentContext() == this)
    {
        auto screenBounds = pdcpp::Graphics::getScreenBounds();
        pdcpp::ScopedGraphicsContext context(screenBounds);
        pdcpp::GlobalPlaydateAPI::get()->graphics->setDrawMode(kDrawModeNXOR);
        pdcpp::Graphics::fillRectangle(screenBounds, kColorWhite);

        std::string msg = "Press  A  to enter crank-able context.";
        const auto font = pdcpp::LookAndFeel::getDefaultLookAndFeel()->getDefaultFont();
        const auto screenCenter = screenBounds.getCenter();
        const auto point = pdcpp::Rectangle<int>(0, 0, font.getTextWidth(msg), font.getFontHeight()).withCenter(screenCenter).getTopLeft();
        auto circleBounds = pdcpp::Rectangle<int>(point.x + font.getTextWidth("Press "), point.y - 1, 20, 20);
        pdcpp::Graphics::fillEllipse(circleBounds, 0, 0);
        font.drawText(msg, point.x, point.y);
        m_RingMenu.redraw();
    }
    return 1;
}

void App::buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released)
{
    if (PDButtons::kButtonA & pressed)
        { pushChildContext(&m_Menu); }
}
