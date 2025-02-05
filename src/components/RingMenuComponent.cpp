//
// Created by Matt on 11/17/2024.
//

#include "pdcpp/components/RingMenuComponent.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"
#include "pdcpp/graphics/LookAndFeel.h"

void pdcpp::RingMenuComponent::drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees)
{
    // TODO: figure out how to draw this as an ellipse
    assert(bounds.height == bounds.width);

    auto center = bounds.getCenter();
    auto innerBounds = bounds.reduced(thickness);
    pdcpp::Graphics::fillEllipse(innerBounds, 0, 0, pdcpp::Colors::white);

    const auto outerPoint = bounds.getCenter().withY(bounds.y);
    const auto innerPoint = innerBounds.getCenter().withY(innerBounds.y);

    auto halfGap = 5.0f;
    auto stepAngle = 360.0f / float(nSplits);

    for (auto i = nSplits; --i >= 0;)
    {
        auto startAngle = stepAngle * float(i) + halfGap + rotationDegrees;
        auto endAngle = startAngle + (stepAngle - halfGap * 2.0f);

        // Fill
        pdcpp::Graphics::drawEllipse(bounds, thickness, startAngle, endAngle, select == i ?pdcpp::Colors::steppedDither5 : pdcpp::Colors::white);

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

pdcpp::RingMenuComponent::RingMenuComponent(
    std::vector<MenuItem> menu, std::function<void()> nonAction, float rotationDegrees)
    : MenuComponentBase(std::move(menu), std::move(nonAction))
    , m_Rotation(rotationDegrees)
{}

void pdcpp::RingMenuComponent::resized(const pdcpp::Rectangle<float>& newBounds)
    { m_PreRenderedImage = buildPreRenderedImage(); }

void pdcpp::RingMenuComponent::draw() { m_PreRenderedImage.draw(getBounds().getTopLeft().toInt()); }

pdcpp::Image pdcpp::RingMenuComponent::buildPreRenderedImage()
{
    //TODO: do a look and feel thing!
    auto thickness = 25;
    auto gap = 10;
    auto bounds = getLocalBounds();
    auto menu = getMenuItems();

    return pdcpp::Image::drawAsImage(bounds, [&]()
    {
        drawSplitCircle(bounds.toInt(), thickness, menu.size(), getSelectedIndex(), m_Rotation);

        auto stepAngle = 360.0f / float(menu.size());
        auto currentAngle = m_Rotation + (stepAngle / 2); // added half-rotation to synchronize the labels to their segments

        for (auto& item : menu)
        {
            auto iconBounds = item.getBounds(getLookAndFeel()->getDefaultFont());
            iconBounds = iconBounds.withEdgeInEllipse(bounds.reduced(thickness + gap),  pdcpp::degToRad(currentAngle - 90));

            std::visit(
                Overload
                {
                    [&](std::string& str)
                    {
                        auto font = getLookAndFeel()->getDefaultFont();
                        std::ignore = font.drawWrappedText(str, iconBounds, Font::Justification::Center);
                    },
                    [&](Component* component)
                    {
                        component->setBounds(iconBounds);
                        component->redraw();
                    }
                }, item.icon
            );

            currentAngle += stepAngle;
        }
    });
}

void pdcpp::RingMenuComponent::selectedChanged()
{
    m_PreRenderedImage = buildPreRenderedImage();
}
