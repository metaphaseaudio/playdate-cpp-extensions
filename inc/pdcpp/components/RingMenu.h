//
// Created by Matt on 11/17/2024.
//

#pragma once
#include "MenuComponentBase.h"

namespace pdcpp
{
    class RingMenuComponent
        : public MenuComponentBase
    {
    public:
        explicit RingMenuComponent(
                std::vector<MenuItem> menu, std::function<void()> nonAction=[](){}, float rotationDegrees = 0.0f);

        static void drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees);

    private:
        Image buildPreRenderedImage() override;
        float m_Rotation;
    };
}
