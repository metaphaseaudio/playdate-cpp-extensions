//
// Created by Matt on 11/17/2024.
//

#pragma once
#include <variant>
#include <pdcpp/components/Component.h>

namespace pdcpp
{
    class RingMenuComponent
        : public pdcpp::Component
    {
    public:
        explicit RingMenuComponent(
            std::vector<std::variant<std::string, pdcpp::Component*>> icons,
            float rotationDegrees = 0.0f
        );

        static void drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees);

    protected:
        void resized(const Rectangle<float>& newBounds) override;

        void draw() override;

    private:
        void updatePreRenderedImage();

        std::vector<std::variant<std::string, pdcpp::Component*>> m_Icons;
        pdcpp::Image m_PreRenderedImage;
        float m_Rotation;
        int m_Selected;
    };
}
