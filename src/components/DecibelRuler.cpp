//
// Created by Matt on 2/15/2025.
//

#include <pdcpp/components/DecibelRuler.h>
#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/graphics/LookAndFeel.h>

pdcpp::DecibelRuler::DecibelRuler(int divisions, bool horizontal, bool invertMarkings)
    : m_Horizontal(horizontal)
{
    assert(divisions > 0);
    for (int i = divisions; --i >= 0;)
    {
        auto& index = m_Indexes.emplace_back(std::make_unique<Indicator>(horizontal, invertMarkings));
        addChildComponent(index.get());
    }
}

void pdcpp::DecibelRuler::resized(const pdcpp::Rectangle<float>& newBounds)
{
    const auto topRight = newBounds.getTopRight();
    const auto bottomLeft = newBounds.getBottomLeft();
    const auto max = std::log10(m_Horizontal ? topRight.x : bottomLeft.y);
    const auto min = std::log10(m_Horizontal ? bottomLeft.x : topRight.y);
    const auto diff = max - min;
    const auto stepSize = diff / (m_Indexes.size() - 1);
    const auto font = getLookAndFeel()->getDefaultFont();
    const auto indicatorBounds = pdcpp::Rectangle<int>(
        0, 0,
        m_Horizontal ? font.getTextWidth("0000") : newBounds.width,
        m_Horizontal ? newBounds.height : font.getFontHeight()
    );

    for (int i = 0; i < m_Indexes.size(); i++)
    {
        const auto mark_offset =  float(i) * stepSize;
        const auto gain = 1.0f - float(i) / float(m_Indexes.size() - 1);
        const auto gain_db = pdcpp::gainToDB(gain);
        const auto int_mark_offset = int(std::pow(10, mark_offset + min));
        const auto text = pdcpp::to_string_with_precision(gain_db, 0);

        auto& index = m_Indexes.at(i);

        index->setText(text);
        index->setBounds(
            indicatorBounds.withCenter(
                m_Horizontal ? pdcpp::Point<int>(int_mark_offset, newBounds.getCenter().y)
                             : pdcpp::Point<int>(newBounds.getCenter().x, int_mark_offset)
            ).toFloat()
        );
    }
}

////////////////////////////////////////////////////////////////////////////////

pdcpp::DecibelRuler::Indicator::Indicator(bool horizontal, bool invertMarkings)
        : m_Horizontal(horizontal)
        , m_InvertMarkings(invertMarkings)
        , m_IndicatorStart({0, 0})
        , m_IndicatorEnd({0, 0})
{}

void pdcpp::DecibelRuler::Indicator::draw()
{
    Component::draw();
    const auto font = getLookAndFeel()->getDefaultFont();
    font.drawText(m_Text, m_TextArea.x, m_TextArea.y);
    pdcpp::Graphics::drawLine(m_IndicatorStart, m_IndicatorEnd, 1);
}

void pdcpp::DecibelRuler::Indicator::resized(const pdcpp::Rectangle<float>& newBounds)
{
    Component::resized(newBounds);
    const auto laf = getLookAndFeel();
    const auto font = laf->getDefaultFont();
    m_TextArea = font.getTextArea(m_Text).withCenter(newBounds.getCenter().toInt());

    if (m_Horizontal)
    {
        if (m_InvertMarkings)
        {
            m_TextArea.alignTop(newBounds.y);
            m_IndicatorStart = newBounds.getTopCenter().toInt();
            m_IndicatorEnd = m_TextArea.getTopCenter() + pdcpp::Point<int>{0, -2};
        }
        else
        {
            m_TextArea.alignBottom(newBounds.y + newBounds.height);
            m_IndicatorStart = m_TextArea.getBottomCenter() + pdcpp::Point<int>{0, 2};
            m_IndicatorEnd = newBounds.getBottomCenter().toInt();;
        }
    }
    else
    {
        if (m_InvertMarkings)
        {
            m_TextArea.alignRight(newBounds.x + newBounds.width);
            m_IndicatorStart = newBounds.getLeftCenter().toInt();
            m_IndicatorEnd = m_TextArea.getLeftCenter() + pdcpp::Point<int>{-2, 0};
        }
        else
        {
            m_TextArea.alignLeft(newBounds.x);
            m_IndicatorStart = m_TextArea.getRightCenter() + pdcpp::Point<int>{2, 0};
            m_IndicatorEnd = newBounds.getRightCenter().toInt();
        }
    }
}
