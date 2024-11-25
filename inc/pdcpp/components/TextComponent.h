//
// Created by Matt on 11/23/2024.
//

#pragma once
#include <pdcpp/components/Component.h>
#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/graphics/Colors.h>
#include <pdcpp/graphics/Border.h>
#include "pdcpp/graphics/Font.h"


namespace pdcpp
{
    class TextComponent
        : public pdcpp::Component
    {
    public:
        enum ColorIds
            { backgroundColorId = 0x1000280, textColorId = 0x1000281, outlineColorId = 0x1000282, };

        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;
            virtual void drawTextComponent(const TextComponent& text) = 0;
        };

        explicit TextComponent(
            std::string text,
            pdcpp::Font::Justification justification=pdcpp::Font::Justification::Center,
            pdcpp::Font::VerticalJustification verticalJustification=pdcpp::Font::VerticalJustification::Middle,
            PDStringEncoding encoding=kASCIIEncoding
        );

        void setText(std::string newText, PDStringEncoding encoding=kASCIIEncoding)
            { m_Text = newText; m_Encoding = encoding;}
        void setJustification(pdcpp::Font::Justification justification) { m_Justification = justification; }
        void setVerticalJustification(pdcpp::Font::VerticalJustification verticalJustification)
            { m_VerticalJustification = verticalJustification; }
        void setBorderSize(pdcpp::Border<int> borderSize) { m_Border = borderSize; };

        [[ nodiscard ]] std::string getText() const noexcept { return m_Text; }
        [[ nodiscard ]] pdcpp::Font::Justification getJustification() const noexcept { return m_Justification; }
        [[ nodiscard ]] pdcpp::Font::VerticalJustification getVerticalJustification() const noexcept
            { return m_VerticalJustification; }
        [[ nodiscard ]] PDStringEncoding getEncoding() const noexcept { return m_Encoding; }
        [[ nodiscard ]] Border<int> getBorder() const noexcept { return m_Border; }

    protected:
        void draw() override;

        std::string m_Text;
        PDStringEncoding m_Encoding;
        pdcpp::Font::Justification m_Justification;
        pdcpp::Font::VerticalJustification m_VerticalJustification;
        pdcpp::Border<int> m_Border;
    };
}
