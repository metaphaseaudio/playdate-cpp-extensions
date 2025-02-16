//
// Created by Matt on 2/15/2025.
//

#include <pdcpp/components/DecibelRuler.h>
#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/graphics/LookAndFeel.h>

pdcpp::DecibelRuler::DecibelRuler(int divisions, bool horizontal, bool invertMarkings)
    : m_Divisions(divisions)
    , m_Horizontal(horizontal)
    , m_InvertMarkings(invertMarkings)
{}

void pdcpp::DecibelRuler::draw()
{
    const auto bounds = getBounds();
    const auto third = (m_Horizontal ? bounds.height : bounds.width) / 3.0f;
    const auto start = m_Horizontal ? bounds.y : bounds.x;
    const auto end = m_Horizontal ? bounds.y + bounds.height : bounds.x + bounds.width;

    auto laf = getLookAndFeel();
    auto font = laf->getDefaultFont();

    for (auto [text, index] : m_Indexes)
    {
        const auto a = m_Horizontal ? pdcpp::Point<int>(index, start) :  pdcpp::Point<int>(start, index);
        const auto b = m_Horizontal ? pdcpp::Point<int>(index, start + third) :  pdcpp::Point<int>(start + third, index);
        const auto c = m_Horizontal ? pdcpp::Point<int>(index, start + third + third) : pdcpp::Point<int>(start + third + third, index);
        const auto d = m_Horizontal ? pdcpp::Point<int>(index, end) : pdcpp::Point<int>(end, index);

        auto textArea = font.getTextArea(text);
        textArea = textArea.withCenter(m_InvertMarkings ? c : b);
        font.drawText(text, textArea.x, textArea.y);

        const auto edge = pdcpp::Point<int>(
            textArea.getCenter().x + (m_Horizontal ? 0 : (textArea.width / 2.0f + 3.0f) * (m_InvertMarkings ? -1 : 1)),
            textArea.getCenter().y + (m_Horizontal ? (textArea.height / 2.0f) *  (m_InvertMarkings ? 1 : -1) : 0)
        );

        const auto mark_a = m_InvertMarkings ? a : edge;
        const auto mark_b = m_InvertMarkings ? edge : d;
        pdcpp::Graphics::drawLine(mark_a, mark_b, 1);
    }
}

void pdcpp::DecibelRuler::resized(const pdcpp::Rectangle<float>& newBounds)
{
    const auto topRight = newBounds.getTopRight();
    const auto bottomLeft = newBounds.getBottomLeft();
    const auto max = std::log10(m_Horizontal ? topRight.x : bottomLeft.y);
    const auto min = std::log10(m_Horizontal ? bottomLeft.x : topRight.y);
    const auto diff = max - min;
    const auto stepSize = diff / m_Divisions;

    for (int i = 0; i <= m_Divisions; i++)
    {
        const auto mark_offset =  float(i) * stepSize;
        const auto gain = 1.0f - float(i) / float(m_Divisions);
        const auto gain_db = pdcpp::gainToDB(gain);
        const auto text = pdcpp::to_string_with_precision(gain_db, 0);
        m_Indexes.emplace_back(Indicator{text, int(std::pow(10, mark_offset + min))});
    }
}
