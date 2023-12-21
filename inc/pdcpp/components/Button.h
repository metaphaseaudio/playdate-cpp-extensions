//
// Created by Matt on 12/19/2023.
//

#pragma once

#include <functional>
#include "Component.h"

namespace pdcpp
{
    class Button
        : public pdcpp::Component
    {
    public:
        explicit Button(std::function<void()> pressFunc=[](){}, std::function<void()> releaseFunc=[](){}, bool focused=false);
        virtual void press(bool shouldPress);
        void focus(bool shouldFocus) { m_Focused = shouldFocus; };
        
        [[ nodiscard ]] bool isPressed() const { return m_Pressed; };
        [[ nodiscard ]] bool isFocused() const { return m_Focused; };
        
        std::function<void()> onPress;
        std::function<void()> onRelease;

    protected:
        void draw() override;
        void resized(const Rectangle<float>& newBounds) override;

    private:
        bool m_Pressed, m_Focused;
    };

    class ToggleButton
        : public Button
    {
    public:
        explicit ToggleButton(bool startingToggleState);
        void press(bool shouldPress) override;

    protected:
        void draw() override;

    private:
        bool m_ToggleState;
    };
}