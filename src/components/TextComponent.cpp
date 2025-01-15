//
// Created by Matt on 11/23/2024.
//

#include "pdcpp/components/TextComponent.h"
#include "pdcpp/graphics/LookAndFeel.h"


pdcpp::TextComponent::TextComponent(
        std::string text,
        pdcpp::Font::Justification justification,
        pdcpp::Font::VerticalJustification verticalJustification,
        PDStringEncoding encoding
)
    : m_Text(std::move(text))
    , m_Border{1, 1, 5, 5}
    , m_Justification(justification)
    , m_VerticalJustification(verticalJustification)
    , m_Encoding(encoding)
{};

void pdcpp::TextComponent::draw()
{
    getLookAndFeel()->drawTextComponent(*this);
}

pdcpp::Font& pdcpp::TextComponent::getFont() const
{
    if (p_FontOverride != nullptr ) { return *p_FontOverride; }
    return getLookAndFeel()->getDefaultFont();
}
