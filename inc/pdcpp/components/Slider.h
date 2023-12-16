//
// Created by Matt on 12/4/2023.
//

#pragma once
#include <functional>
#include "Component.h"

namespace pdcpp
{
    class Slider
        : public pdcpp::Component
    {
    public:
        class Listener
        {
        public:
            virtual void sliderValueChanged(Slider* sliderThatChanged) = 0;
        };

        Slider(float min, float max, float startingValue, int nSteps=20);
        ~Slider();

        void setValue(float newValue, bool notify=true);
        [[ nodiscard ]] float getValue() const;

        void setMin(float minValue);
        void setMax(float maxValue);
        [[ nodiscard ]] float getMin() const;
        [[ nodiscard ]] float getMax() const;

        void operator++(int) { setValue(getValue() + m_IncrementSize); }// return *this; }
        void operator--(int) { setValue(getValue() - m_IncrementSize); }// return *this; }

        std::function<void(float)> onChange;

    protected:
        void draw() override;
        void notifyListeners();

    private:
        float m_CurrentValue, m_Min, m_Max, m_IncrementSize;
        std::vector<Listener*> m_Listeners;
    };
}
