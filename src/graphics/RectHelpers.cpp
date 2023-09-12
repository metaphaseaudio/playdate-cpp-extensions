/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/28/2023
 *  Original author: MrBZapp
 */

#include <algorithm>
#include <pdcpp/graphics/RectHelpers.h>

PDRect pdcpp::RectHelpers::getOverlappingRect(const PDRect& x, const PDRect& y)
{
    const auto overlapStartX = std::max(y.x, x.x);
    const auto overlapEndX = std::min(y.x + y.width, x.x + x.width);
    const auto overlapLenX = std::max(0.0f, overlapEndX - overlapStartX);

    const auto overlapStartY = std::max(y.y, x.y);
    const auto overlapEndY = std::min(y.y + y.height, x.y + x.height);
    const auto overlapLenY = std::max(0.0f, overlapEndY - overlapStartY);

    return {
        overlapLenX > 0 ? overlapStartX : 0.0f,
        overlapLenY > 0 ? overlapStartY : 0.0f,
        overlapLenX,
        overlapLenY
    };
}
