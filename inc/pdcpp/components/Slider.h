//
// Created by Matt on 12/4/2023.
//

#pragma once
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

        Slider(float min, float max, float startingValue);
        ~Slider();

        void setValue(float newValue);
        [[ nodiscard ]] float getValue() const;

        void setMin(float minValue);
        void setMax(float maxValue);
        [[ nodiscard ]] float getMin() const;
        [[ nodiscard ]] float getMax() const;

    protected:
        void draw() override;
        void notifyListeners();

    protected:
        float m_CurrentValue, m_Min, m_Max;
        std::vector<Listener*> m_Listeners;
    };
}
