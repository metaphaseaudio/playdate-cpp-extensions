//
// Created by Matt on 12/9/2023.
//

#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

void pdcpp::Graphics::drawRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, LCDColor color)
{

}

void pdcpp::Graphics::fillRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, LCDColor color)
{

}

void pdcpp::Graphics::drawDashedLine
(const pdcpp::Point<int>& start, const pdcpp::Point<int>& end, const pdcpp::Graphics::Dash* dashes, int nDashes, int weightPx, int startDashIndex)
{
    const auto distance = start.distance(end);
    const auto delta = end - start;

    const float stepAlpha = 1.0f / distance;
    auto i = startDashIndex;

    for (float alpha = 0.0; alpha < 1.0;)
    {
        const auto lastAlpha = alpha;
        const auto dash = dashes[i];
        alpha += std::min(1.0f, dash.length * stepAlpha);
        i = ++i % nDashes;

        const auto a = start.toFloat() + (delta.toFloat() * lastAlpha);
        const auto b = start.toFloat() + (delta.toFloat() * alpha);

        pdcpp::GlobalPlaydateAPI::get()->graphics->drawLine(a.x, a.y, b.x, b.y, weightPx, dash.color);
    }
}

pdcpp::Rectangle<float> pdcpp::Graphics::getScreenBounds()
{
    return {
        0, 0,
        float(pdcpp::GlobalPlaydateAPI::get()->display->getWidth()),
        float(pdcpp::GlobalPlaydateAPI::get()->display->getHeight())
    };
}

void pdcpp::Graphics::fillPolygon(const std::vector<pdcpp::Point<float>>& points,  LCDColor color, LCDPolygonFillRule fillRule)
{
    std::vector<int> intPoints;
    for (auto& point : points)
    {
        intPoints.push_back(point.x);
        intPoints.push_back(point.y);
    }

    pdcpp::GlobalPlaydateAPI::get()->graphics->fillPolygon(intPoints.size() / 2.0f, intPoints.data(), color, fillRule);
}

void pdcpp::Graphics::drawPolygon(const std::vector<pdcpp::Point<float>>& points, int thickness, LCDColor color)
{
    for (int i = 0; i < points.size() - 1; i++)
    {
        const auto start = points.at(i);
        const auto end = points.at(i + 1);
        pdcpp::GlobalPlaydateAPI::get()->graphics->drawLine(start.x, start.y, end.x, end.y, thickness, color);
    }
}

