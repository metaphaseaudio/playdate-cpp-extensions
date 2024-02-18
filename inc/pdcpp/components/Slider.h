/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/4/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <functional>
#include "Component.h"

namespace pdcpp
{
    class Slider
        : public pdcpp::Component
    {
    public:
        /**
         * Listeners get the same message that is sent to the `onChange`
         * callback, but you can use more than one.
         */
        class Listener
        {
        public:
            // Called with a pointer to the slider that changed
            virtual void sliderValueChanged(Slider* sliderThatChanged) = 0;
        };

        enum SliderStyle
        {
            Horizontal,
            Vertical,
            Rotary,
            Numeric
        };

        /**
         * Creates a Slider component. Actual drawing of the slider is delegated
         * to the LookAndFeel.
         *
         * @param min The minimum value of the slider
         * @param max the maximum value of the slider
         * @param startingValue the starting value of the slider
         * @param nSteps the granularity of the slider. default 20 steps from
         *     min to max.
         */
        Slider(float min, float max, float startingValue, int nSteps=20, SliderStyle style=Horizontal);

        ~Slider();

        /**
         * Sets the value of the slider
         *
         * @param newValue the new Value of the slider
         * @param notify whether the listeners and `onChange` callback should be
         *     notified of the new value, to avoid stack overflow if the value
         *     is set from within a a listener or `onChange`. Notification is on
         *     by default.
         */
        void setValue(float newValue, bool notify=true);

        /**
         * @returns the current value of the slider
         */
        [[ nodiscard ]] float getValue() const;

        /**
         * Change the minimum value of the slider
         * @param minValue new minimum value
         */
        void setMin(float minValue);

        /**
         * Change the maximum value of the slider
         * @param maxValue new maximum value
         */
        void setMax(float maxValue);

        void setStyle(SliderStyle style);

        /**
         * @returns the current minimum value of the slider
         */
        [[ nodiscard ]] float getMin() const;


        /**
         * @returns the current maximum value of the slider
         */
        [[ nodiscard ]] float getMax() const;

        [[ nodiscard ]] SliderStyle getStyle() const;

        /**
         * Increment the slider by one increment size
         */
        void operator++(int) { setValue(getValue() + m_IncrementSize); }

        /**
         * Decrement the slider by one increment size
         */
        void operator--(int) { setValue(getValue() - m_IncrementSize); }

        /**
         * Callback for when the slider changes. Will be called with the new
         * value.
         */
        std::function<void(float)> onChange;

    protected:
        void redrawCachedImage();
        void draw() override;
        void resized(const Rectangle<float>& newBounds) override;
        void notifyListeners();

    private:
        SliderStyle m_Style;
        float m_CurrentValue, m_Min, m_Max, m_IncrementSize;
        std::vector<Listener*> m_Listeners;
        pdcpp::Image m_CachedImage;
    };
}
