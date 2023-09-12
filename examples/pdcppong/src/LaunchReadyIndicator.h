/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pdcpp/graphics/Sprite.h>
#include <pdcpp/graphics/Font.h>


namespace pdcppong
{
    class LaunchReadyIndicator
        : public pdcpp::Sprite
    {
    public:
        LaunchReadyIndicator();

        void redraw(PDRect bounds, PDRect drawrect) override;
    private:
        pdcpp::Font m_Font;
    };
}
