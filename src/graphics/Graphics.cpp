/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/9/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/graphics/Graphics.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

void pdcpp::Graphics::drawRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, int linePx, LCDColor color)
{
    auto cornerCenters = bounds.reduced(radius);

    // left/right
    auto lBounds = cornerCenters.withOrigin(cornerCenters.getTopLeft() - pdcpp::Point<int>(radius, 0));
    pdcpp::Graphics::drawLine(lBounds.getTopLeft(), lBounds.getBottomLeft(), linePx, color);

    auto rBounds = cornerCenters.withOrigin(cornerCenters.getTopLeft() + pdcpp::Point<int>(radius, 0));
    pdcpp::Graphics::drawLine(rBounds.getTopRight(), rBounds.getBottomRight(), linePx, color);

    auto tBounds = cornerCenters.withOrigin(cornerCenters.getTopLeft() - pdcpp::Point<int>(0, radius));
    pdcpp::Graphics::drawLine(tBounds.getTopLeft(), tBounds.getTopRight(), linePx, color);

    auto bBounds = cornerCenters.withOrigin(cornerCenters.getTopLeft() + pdcpp::Point<int>(0, radius));
    pdcpp::Graphics::drawLine(bBounds.getBottomLeft(), bBounds.getBottomRight(), linePx, color);

    auto ellipseBounds = pdcpp::Rectangle<int>(0, 0, radius + radius, radius + radius);
    pdcpp::Graphics::drawEllipse(ellipseBounds.withCenter(cornerCenters.getTopLeft()), linePx, 270, 0, color);
    pdcpp::Graphics::drawEllipse(ellipseBounds.withCenter(cornerCenters.getTopRight()), linePx, 0, 90, color);
    pdcpp::Graphics::drawEllipse(ellipseBounds.withCenter(cornerCenters.getBottomRight()), linePx, 90, 180, color);
    pdcpp::Graphics::drawEllipse(ellipseBounds.withCenter(cornerCenters.getBottomLeft()), linePx, 180, 270, color);
}

void pdcpp::Graphics::fillRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, LCDColor color)
{
    const auto cornerCenters = bounds.reduced(radius);
    const auto cornerBounds = pdcpp::Rectangle<int>(0, 0, radius + radius, radius + radius);

    const auto corner1 = cornerBounds.withCenter(cornerCenters.getTopLeft());
    const auto corner2 = cornerBounds.withCenter(cornerCenters.getTopRight());
    const auto corner3 = cornerBounds.withCenter(cornerCenters.getBottomRight());
    const auto corner4 = cornerBounds.withCenter(cornerCenters.getBottomLeft());

    const auto fill1 = pdcpp::Rectangle<int>(
            corner1.getCenter() - pdcpp::Point<int>(0, radius), corner3.getCenter() + pdcpp::Point<int>(0, radius));
    const auto fill2 = pdcpp::Rectangle<int>(
            corner1.getCenter() - pdcpp::Point<int>(radius, 0), corner3.getCenter() + pdcpp::Point<int>(radius, 0));

    pdcpp::Graphics::fillRectangle(fill1 , color);
    pdcpp::Graphics::fillRectangle(fill2 , color);

    pdcpp::Graphics::fillEllipse(corner1, 270, 0, color);
    pdcpp::Graphics::fillEllipse(corner2, 0, 90, color);
    pdcpp::Graphics::fillEllipse(corner3, 90, 180, color);
    pdcpp::Graphics::fillEllipse(corner4, 180, 270, color);


}

void pdcpp::Graphics::drawDashedLine
(const pdcpp::Point<int>& start, const pdcpp::Point<int>& end, const std::vector<pdcpp::Graphics::Dash>& dashes, int weightPx, int startDashIndex)
{
    const auto distance = start.distance(end);
    const auto delta = end - start;

    const float stepAlpha = 1.0f / distance;

    for (float alpha = 0.0; alpha < 1.0f;)
    {
        const auto lastAlpha = alpha;
        const auto dash = dashes[startDashIndex];
        alpha += std::min(1.0f, dash.length * stepAlpha);
        startDashIndex = ++startDashIndex % dashes.size();

        const auto a = start.toFloat() + (delta.toFloat() * lastAlpha);
        const auto b = start.toFloat() + (delta.toFloat() * alpha);

        pdcpp::GlobalPlaydateAPI::get()->graphics->drawLine(a.x, a.y, b.x, b.y, weightPx, dash.color);
    }
}

pdcpp::Rectangle<int> pdcpp::Graphics::getScreenBounds()
{
    return {
        0, 0,
        pdcpp::GlobalPlaydateAPI::get()->display->getWidth(),
        pdcpp::GlobalPlaydateAPI::get()->display->getHeight()
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

void pdcpp::Graphics::drawRectangle(const pdcpp::Rectangle<int>& rect, LCDColor color)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->drawRect(rect.x, rect.y, rect.width, rect.height, color);
}

void pdcpp::Graphics::fillRectangle(const pdcpp::Rectangle<int>& rect, LCDColor color)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->fillRect(rect.x, rect.y, rect.width, rect.height, color);
}

void pdcpp::Graphics::drawEllipse(const pdcpp::Rectangle<int>& rect, int lineThickness, float startAngle, float endAngle, LCDColor color)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->drawEllipse(rect.x, rect.y, rect.width, rect.height, lineThickness, startAngle, endAngle, color);
}

void pdcpp::Graphics::fillEllipse(const pdcpp::Rectangle<int>& rect, float startAngle, float endAngle, LCDColor color)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->fillEllipse(rect.x, rect.y, rect.width, rect.height, startAngle, endAngle, color);
}

void pdcpp::Graphics::drawLine(const pdcpp::Point<int>& a, const pdcpp::Point<int>& b, int px, LCDColor color)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->drawLine(a.x, a.y, b.x, b.y, px, color);
}

LCDBitmapDrawMode pdcpp::Graphics::setDrawMode(LCDBitmapDrawMode drawMode)
{
    return pdcpp::GlobalPlaydateAPI::get()->graphics->setDrawMode(drawMode);
}

void pdcpp::Graphics::setClipRect(const pdcpp::Rectangle<int>& clipRect)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->setClipRect(clipRect.x, clipRect.y, clipRect.width, clipRect.height);
}

void pdcpp::Graphics::clearClipRect()
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->clearClipRect();
}

void pdcpp::Graphics::setLineCapStyle(LCDLineCapStyle endCapStyle)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->setLineCapStyle(endCapStyle);
}

void pdcpp::Graphics::fillTriangle(const pdcpp::Point<int>& a, const pdcpp::Point<int>& b, const pdcpp::Point<int>& c, LCDColor color)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->fillTriangle(a.x, a.y, b.x, b.y, c.x, c.y, color);
}
