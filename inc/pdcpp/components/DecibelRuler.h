//
// Created by Matt on 2/15/2025.
//

#pragma once
#include <pdcpp/components/Component.h>

namespace pdcpp
{
    class DecibelRuler
        : public pdcpp::Component
    {
    public:
        explicit DecibelRuler(int divisions=6, bool horizontal=false, bool invertMarkings=false);
        void resized(const pdcpp::Rectangle<float>& newBounds) override;

    private:
        class Indicator
            : public pdcpp::Component
        {
        public:
            explicit Indicator(bool horizontal, bool invertMarkings);
            void setText(std::string text) { m_Text = std::move(text); };

        private:
            void draw() override;
            void resized(const Rectangle<float>& newBounds) override;

            std::string m_Text;
            pdcpp::Rectangle<int> m_TextArea;
            pdcpp::Point<int> m_IndicatorStart, m_IndicatorEnd;
            const bool m_Horizontal, m_InvertMarkings;
        };

        bool m_Horizontal;
        std::vector<std::unique_ptr<Indicator>> m_Indexes;
    };
}
