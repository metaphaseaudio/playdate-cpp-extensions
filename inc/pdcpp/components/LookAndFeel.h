//
// Created by Matt on 12/4/2023.
//

#pragma once
#include <pd_api.h>
#include <pdcpp/graphics/Rectangle.h>

#include <pdcpp/graphics/Font.h>

namespace pdcpp
{

    class LookAndFeel
    {
    public:
        LookAndFeel();

        virtual void drawSlider(const playdate_graphics* g, const pdcpp::Rectangle<float>& bounds, float min, float max, float value);

        void setDefaultFont(Font newFont);
        pdcpp::Font& getDefaultFont();

        static void setDefaultLookAndFeel(LookAndFeel* newLAF);
        static LookAndFeel* getDefaultLookAndFeel();

    private:
        Font m_DefaultFont;
    };

} // pdcpp
