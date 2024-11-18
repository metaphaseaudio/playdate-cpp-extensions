//
// Created by Matt on 11/17/2024.
//

#pragma once
#include <pdcpp/core/InputContext.h>
#include "pdcpp/graphics/Sprite.h"

namespace pdcpp
{
    class RingMenu
        : public pdcpp::InputContext
        , public pdcpp::Sprite
    {
    public:
        RingMenu(
            pdcpp::InputContextManager& contextManager,
            std::vector<std::function<void()>> actions,
            float rotationDegrees = 0.0f
        );


        static void drawSplitCircle(const pdcpp::Rectangle<int>& bounds, int thickness, int nSplits, int select, float rotationDegrees);

    };
}
