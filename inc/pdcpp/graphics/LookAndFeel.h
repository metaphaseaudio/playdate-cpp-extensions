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
#include <map>
#include <pd_api.h>
#include <pdcpp/graphics/Rectangle.h>

#include <pdcpp/graphics/Font.h>

namespace pdcpp
{

    class LookAndFeel
    {
    public:
        LookAndFeel();
        virtual ~LookAndFeel() = default;

        virtual void drawSlider(const playdate_graphics* g, const pdcpp::Rectangle<float>& bounds, float min, float max, float value);
        void setDefaultFont(Font newFont);
        pdcpp::Font& getDefaultFont();

        static void setDefaultLookAndFeel(LookAndFeel* newLAF);
        static LookAndFeel* getDefaultLookAndFeel();

    private:
        Font m_DefaultFont;
        static LookAndFeel* defaultLookAndFeel;
    };

}