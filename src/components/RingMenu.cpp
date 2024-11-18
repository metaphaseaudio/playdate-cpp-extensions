//
// Created by Matt on 11/17/2024.
//

#include "pdcpp/components/RingMenu.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"


//void pdcpp::RingMenu::drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees)
//{
//    // This has to be a circle.
//    assert(bounds.height == bounds.width);
//
//    auto center = bounds.getCenter();
//    auto innerBounds = bounds.reduced(thickness);
//    pdcpp::Graphics::fillEllipse(innerBounds, 0, 0, kColorWhite);
//
//    const auto outerPoint = bounds.getCenter().withY(bounds.y);
//    const auto innerPoint = innerBounds.getCenter().withY(innerBounds.y);
//
//    auto halfGap = 10.0f;
//    auto stepAngle = 360.0f / float(nSplits);
//
//    for (auto i = nSplits; --i >= 0;)
//    {
//        auto startAngle = stepAngle * float(i) + halfGap - rotationDegrees;
//        auto endAngle = startAngle + (stepAngle - halfGap * 2.0f);
//
//        // Fill
//        pdcpp::Graphics::drawEllipse(bounds, thickness, startAngle, endAngle, select == i ? pdcpp::Colors::steppedDither5 : kColorWhite);
//
//        pdcpp::Graphics::drawEllipse(bounds, 2, startAngle, endAngle);
//        pdcpp::Graphics::drawEllipse(innerBounds, 2, startAngle, endAngle);
//
//        auto lineStart = outerPoint.rotated(center, pdcpp::degToRad(startAngle));
//        auto lineEnd = innerPoint.rotated(center, pdcpp::degToRad(startAngle));
//        pdcpp::Graphics::drawLine(lineStart, lineEnd, 2);
//
//        lineStart = outerPoint.rotated(center, pdcpp::degToRad(endAngle));
//        lineEnd = innerPoint.rotated(center, pdcpp::degToRad(endAngle));
//        pdcpp::Graphics::drawLine(lineStart, lineEnd, 2);
//    }
//}


void pdcpp::RingMenu::drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees)
{
    auto center = bounds.getCenter();
    auto localBounds = bounds.withOrigin({0, 0});
    auto stepAngle = 360.0f / float(nSplits);
    auto circle = pdcpp::Image::drawAsImage(localBounds, [&](auto graphics){
        auto innerBounds = localBounds.reduced(thickness);
        pdcpp::Graphics::fillEllipse(innerBounds, 0, 0, kColorWhite);

        for (auto i = nSplits; --i >= 0;)
        {
            auto startAngle = stepAngle * float(i) - rotationDegrees;
            auto endAngle = startAngle + stepAngle;

            // Fill
            pdcpp::Graphics::drawEllipse(localBounds, thickness, startAngle, endAngle, select == i ? pdcpp::Colors::steppedDither5 : kColorWhite);
            pdcpp::Graphics::drawEllipse(localBounds, 2, startAngle, endAngle);
            pdcpp::Graphics::drawEllipse(innerBounds, 2, startAngle, endAngle);
        }
    });

    auto lines = pdcpp::Image::drawAsImage(localBounds, [&](auto graphics){
        for (auto i = nSplits; --i >= 0;)
        {
            auto startAngle = stepAngle * float(i) - rotationDegrees;
            auto endAngle = startAngle + stepAngle;

            pdcpp::Rectangle<int> gap = {0, 0, thickness, thickness};


            pdcpp::Graphics::fillRectangle(gap.withCenter())
        }
    });

    circle.draw(bounds.getTopLeft());


}