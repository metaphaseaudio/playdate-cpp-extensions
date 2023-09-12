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
#include "SpriteTags.h"

namespace pdcppong
{
    class BlockSprite
        : public pdcpp::Sprite
    {
    public:
        BlockSprite(SpriteTag boundaryType, const PDRect& bounds);
        void redraw(PDRect bounds, PDRect drawrect) override;

        SpriteCollisionResponseType handleCollision(Sprite* other) override;
    };
}
