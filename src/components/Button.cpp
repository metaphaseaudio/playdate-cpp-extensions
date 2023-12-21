//
// Created by Matt on 12/19/2023.
//

#include <utility>

#include "pdcpp/components/Button.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"

void pdcpp::Button::draw()
{
    auto laf = getLookAndFeel();
    laf->drawButton(pdcpp::GlobalPlaydateAPI::get()->graphics, getBounds(), m_Focused, m_Pressed);
}

void pdcpp::Button::resized(const pdcpp::Rectangle<float>& newBounds)
{
    Component::resized(newBounds);
}

pdcpp::Button::Button(std::function<void()> pressFunc, std::function<void()> releaseFunc, bool focused)
    : m_Pressed(false)
    , m_Focused(focused)
    , onPress(std::move(pressFunc))
    , onRelease(std::move(releaseFunc))
{
}

void pdcpp::Button::press(bool shouldPress)
{
    if (shouldPress && !m_Pressed)
    {
        m_Pressed = shouldPress;
        onPress();
    }

    else if (!shouldPress && m_Pressed)
    {
        m_Pressed = shouldPress;
        onRelease();
    }
}


pdcpp::ToggleButton::ToggleButton(bool startingToggleState)
        : m_ToggleState(startingToggleState)
{}

void pdcpp::ToggleButton::press(bool shouldPress)
{
    if (shouldPress && !isPressed()) { m_ToggleState = !m_ToggleState; }
    Button::press(shouldPress);
}

void pdcpp::ToggleButton::draw()
{
    auto laf = getLookAndFeel();
    laf->drawToggleButton(pdcpp::GlobalPlaydateAPI::get()->graphics, getBounds(), isFocused(), isPressed(), m_ToggleState);
}

