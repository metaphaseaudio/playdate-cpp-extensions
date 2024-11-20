/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2024 - Metaphase
 *
 *  Created: 1/06/2024
 *  Original author: MrBZapp
 *
 *  Description:
 *  The following example demonstrates how one can use the following APIs:
 *   * `pdcpp::Component` for isolating drawable UI elements
 *   * `pdcpp::Viewport` for creating scrollable UI elements
 *   * `pdcpp::Graphics` for drawing colors and shapes
 *   * `pdcpp::InputContextManager` and `pdcpp::InputContext` for managing what
 *     elements are listening to the crank and buttons at any given time
 *   * `pdcpp::ScopedGraphicsContext` for managing how elements are drawn
 */
#pragma once
#include <pdcpp/graphics/Sprite.h>
#include <pdcpp/graphics/Font.h>
#include <pdcpp/components/Viewport.h>
#include <pdcpp/core/InputContext.h>
#include "pdcpp/components/RingMenu.h"


/**
 * This Component provides the wrapped text for the cranked context's scrolling
 * viewport.
 */
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

/**
 * This component draws a "button" to tell the user they can exit by pressing
 * the B button.
 */
class ExitButton
    : public pdcpp::Component
{
public:
    void pressed(bool isPressed) { m_IsPressed = isPressed; redraw(); };
protected:
    void draw() override;
private:
    bool m_IsPressed = false;
};


/**
 * This is a Sprite, meaning the system will handle calling "redraw" when
 * appropriate. It contains the two Components, a viewport and exit button, and
 * is the crank-able input context.
 *
 * When its context is entered it displays itself, and hides it when the context
 * is exited. While in the context, the crank managing the scroll position of
 * the viewport, and conversely, the viewport position will not be updated by
 * the crank when outside of this context.
 */
class CrankContext
    : public pdcpp::Sprite
    , public pdcpp::InputContext
{
public:
    explicit CrankContext(std::string text);
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
    ExitButton m_Button;

};

/**
 * This is the base application which contains the CrankContext, and
 * InputContextManager, is itself an InputContext whose entire purpose is to
 * push the CrankContext as a child context.
 */
class App
    : public pdcpp::InputContext
{
public:
    App();
    int update();

protected:
    void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) override;

private:
    CrankContext m_Menu;
    pdcpp::InputContextManager m_ContextManager;
    pdcpp::RingMenuComponent m_RingMenu;
};
