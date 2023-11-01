/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */

#include "LaunchReadyIndicator.h"
#include <pdcpp/graphics/ScopedGraphicsContext.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcppong::LaunchReadyIndicator::LaunchReadyIndicator()
    : m_Font("/System/Fonts/Roobert-10-Bold.pft")
{
    setBounds({0, 0, float(m_Font.getTextWidth("Press A")) + 3, float(m_Font.getFontHeight())});
    moveTo(200, 140);
}

void pdcppong::LaunchReadyIndicator::redraw(PDRect bounds, PDRect drawrect)
{
    auto context = pdcpp::ScopedGraphicsContext(bounds, kColorWhite);
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    pd->graphics->setDrawMode(LCDBitmapDrawMode::kDrawModeNXOR);
    pd->graphics->drawEllipse(m_Font.getTextWidth("Press") + 1, 0, bounds.height, bounds.height, bounds.height / 2.0f, 0.0f, 0.0f, kColorBlack);
    m_Font.drawText("Press A", 0, 0);
}
