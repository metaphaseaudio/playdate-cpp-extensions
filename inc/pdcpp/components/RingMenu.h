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
        using Icon = std::variant<std::string, pdcpp::Component*>;

        explicit RingMenuComponent(
            std::vector<Icon> icons,
            float rotationDegrees = 0.0f
        );

        static void drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees);

        void setSelected(int i);
        [[ nodiscard ]] int getSelected() const { return m_Selected; }

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
