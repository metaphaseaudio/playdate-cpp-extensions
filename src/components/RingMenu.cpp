//
// Created by Matt on 11/17/2024.
//

#include "pdcpp/components/RingMenu.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"
#include "pdcpp/components/Component.h"
#include <iostream>


void pdcpp::RingMenuComponent::drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees)
{
    // TODO: figure out how to draw this as an ellipse
    assert(bounds.height == bounds.width);

    auto center = bounds.getCenter();
    auto innerBounds = bounds.reduced(thickness);
    pdcpp::Graphics::fillEllipse(innerBounds, 0, 0, kColorWhite);

    const auto outerPoint = bounds.getCenter().withY(bounds.y);
    const auto innerPoint = innerBounds.getCenter().withY(innerBounds.y);

    auto halfGap = 5.0f;
    auto stepAngle = 360.0f / float(nSplits);

    for (auto i = nSplits; --i >= 0;)
    {
        auto startAngle = stepAngle * float(i) + halfGap + rotationDegrees;
        auto endAngle = startAngle + (stepAngle - halfGap * 2.0f);

        // Fill
        pdcpp::Graphics::drawEllipse(bounds, thickness, startAngle, endAngle, select == i ? pdcpp::Colors::steppedDither5 : kColorWhite);

        pdcpp::Graphics::drawEllipse(bounds, 2, startAngle, endAngle);
        pdcpp::Graphics::drawEllipse(innerBounds, 2, startAngle, endAngle);

        auto lineStart = outerPoint.rotated(center, pdcpp::degToRad(startAngle));
        auto lineEnd = innerPoint.rotated(center, pdcpp::degToRad(startAngle));
        pdcpp::Graphics::drawLine(lineStart, lineEnd, 2);

        lineStart = outerPoint.rotated(center, pdcpp::degToRad(endAngle));
        lineEnd = innerPoint.rotated(center, pdcpp::degToRad(endAngle));
        pdcpp::Graphics::drawLine(lineStart, lineEnd, 2);
    }
}

pdcpp::RingMenuComponent::RingMenuComponent(std::vector<std::variant<std::string, pdcpp::Component*>> icons, float rotationDegrees)
    : m_Icons(std::move(icons))
    , m_Rotation(rotationDegrees)
    , m_Selected(-1)
{}

void pdcpp::RingMenuComponent::resized(const pdcpp::Rectangle<float>& newBounds)
{
    updatePreRenderedImage();
}

void pdcpp::RingMenuComponent::draw()
{
    m_PreRenderedImage.draw(getBounds().getTopLeft().toInt());
}

void pdcpp::RingMenuComponent::updatePreRenderedImage()
{
    //TODO: do a look and feel thing!
    auto thickness = 25;
    auto gap = 10;
    auto bounds = getLocalBounds();

    m_PreRenderedImage = pdcpp::Image::drawAsImage(bounds, [&](auto*)
    {
        drawSplitCircle(bounds.toInt(), thickness, m_Icons.size(), m_Selected, m_Rotation);

        auto stepAngle = 360.0f / float(m_Icons.size());
        auto currentAngle = m_Rotation + (stepAngle / 2); // added half-rotation to synchronize the labels to their segments

        for (auto& icon : m_Icons)
        {
            auto iconBounds = std::visit(
                Overload
                {
                    [&](std::string& str) { return getLookAndFeel()->getDefaultFont().getTextArea(str).toFloat(); },
                    [&](Component* component) { return component->getBounds(); }
                }, icon
            );

            iconBounds = iconBounds.withEdgeInEllipse(bounds.reduced(thickness + gap),  pdcpp::degToRad(currentAngle - 90));

            std::visit(
                Overload
                {
                    [&](std::string& str)
                    {
                        auto font = getLookAndFeel()->getDefaultFont();
                        font.drawWrappedText(str, iconBounds, Font::Justification::Center);
                    },
                    [&](Component* component)
                    {
                        component->setBounds(iconBounds);
                        component->redraw();
                    }
                }, icon
            );

            currentAngle += stepAngle;
        }
    });
}

void pdcpp::RingMenuComponent::setSelected(int i)
{
    m_Selected = i;
    updatePreRenderedImage();
}
