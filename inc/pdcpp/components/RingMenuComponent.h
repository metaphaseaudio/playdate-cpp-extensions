//
// Created by Matt on 11/17/2024.
//

#pragma once
#include "MenuComponentBase.h"

namespace pdcpp
{
    class RingMenuComponent
        : public MenuComponentBase
        , public pdcpp::Component
    {
    public:
        explicit RingMenuComponent(
            std::vector<MenuItem> menu, std::function<void()> nonAction=[](){}, float rotationDegrees = 0.0f);

        static void drawSplitCircle(
            const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees);
    protected:
        void resized(const Rectangle<float>& newBounds) override;

        void draw() override;

        void selectedChanged() override;

    private:
        pdcpp::Image buildPreRenderedImage();

        pdcpp::Image m_PreRenderedImage;
        float m_Rotation;
    };
}
