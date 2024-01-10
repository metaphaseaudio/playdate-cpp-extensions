/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/GlobalPlaydateAPI.h>
#include "BlockSprite.h"

pdcppong::BlockSprite::BlockSprite(SpriteTag boundaryType, const pdcpp::Rectangle<float>& bounds)
    : Sprite(boundaryType)
{
    setBounds(bounds);
    setCollideRect({0, 0, bounds.width, bounds.height});
}


void pdcppong::BlockSprite::redraw(const pdcpp::Rectangle<float>& bounds, const pdcpp::Rectangle<float>& drawrect)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->fillRect(bounds.x, bounds.y, bounds.width, bounds.height, kColorBlack);
}

SpriteCollisionResponseType pdcppong::BlockSprite::handleCollision(pdcpp::Sprite* other)
{
    if (other->getTag<SpriteTag>() == WALL) { return kCollisionTypeSlide; }
    return kCollisionTypeOverlap;
}


