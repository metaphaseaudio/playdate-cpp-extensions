//
// Created by Matt on 1/20/2025.
//

#include "pdcpp/components/WaveformViewComponent.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"

void pdcpp::WaveformViewComponent::draw()
{
    auto bounds = getBounds();
    pdcpp::Graphics::drawRectangle(bounds.toInt(), pdcpp::Colors::black);

    auto width = std::max<int>(1, bounds.width / m_Heights.size());
    auto xOffset = bounds.x;

    for (auto h : m_Heights)
    {
        auto height =  std::max<int>(1, bounds.height * std::abs(h));
        pdcpp::Graphics::fillRectangle(
            pdcpp::Rectangle<int>(0, 0, width, height).withCenter(Point<int>(xOffset, bounds.y + (bounds.height / 2.0f))),
            pdcpp::Colors::black
        );
        xOffset += width;
    }
}
