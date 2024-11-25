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
#include <pdcpp/components/TextComponent.h>
#include <pdcpp/graphics/Font.h>

namespace pdcpp
{

    class LookAndFeel
        : public TextComponent::LookAndFeelMethods
    {
    public:
        LookAndFeel();
        ~LookAndFeel() override = default;

        virtual void drawHorizontalSlider(const playdate_graphics* g, const class Slider* slider) const;
        virtual void drawRotarySlider(const playdate_graphics* g, const class Slider* slider) const;
        virtual void drawNumericSlider(const playdate_graphics* g, const class Slider* slider) const;

        void drawTextComponent(const TextComponent& text) override;

        void setColor(int colorID, LCDColor value);
        void resetColor(int colorID);
        [[ nodiscard ]] LCDColor findColor(int colorID) const;

        void setDefaultFont(Font newFont);
        pdcpp::Font& getDefaultFont();
        [[ nodiscard ]] const pdcpp::Font& getDefaultFont() const;

        static void setDefaultLookAndFeel(LookAndFeel* newLAF);
        static LookAndFeel* getDefaultLookAndFeel();

        pdcpp::Font* getFont(const std::string& fontName);
    private:
        Font m_DefaultFont;
        std::map<int, LCDColor> m_Colors;

        // "Globals"
        static std::map<std::string, Font> g_Fonts;
        static LookAndFeel* defaultLookAndFeel;
    };

}