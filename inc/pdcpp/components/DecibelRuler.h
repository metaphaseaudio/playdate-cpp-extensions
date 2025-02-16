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
        void draw() override;
        void resized(const pdcpp::Rectangle<float>& newBounds) override;

    private:
        struct Indicator
        {
            std::string text;
            int position;
        };

        const int m_Divisions;
        const bool m_Horizontal, m_InvertMarkings;
        std::vector<Indicator> m_Indexes;
    };
}
