/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/28/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/graphics/ScopedGraphicsContext.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::ScopedGraphicsContext::ScopedGraphicsContext(const PDRect& bounds, LCDColor bgColor, bool drawOnExit)
    : m_Bounds(bounds)
    , m_DrawOnExit(drawOnExit)
{
    auto pd = GlobalPlaydateAPI::get();
    m_Context = pd->graphics->newBitmap(int(bounds.width), int(bounds.height), bgColor);
    pd->graphics->pushContext(m_Context);
}

pdcpp::ScopedGraphicsContext::~ScopedGraphicsContext()
{
    auto pd = GlobalPlaydateAPI::get();
    pd->graphics->popContext();
    if (m_DrawOnExit)
        { pd->graphics->drawBitmap(m_Context, int(m_Bounds.x), int(m_Bounds.y), kBitmapUnflipped); }
    pd->graphics->freeBitmap(m_Context);
}

pdcpp::Image pdcpp::ScopedGraphicsContext::getCopyAsImage() const
{
    return Image::copyFromPointer(m_Context);
}
