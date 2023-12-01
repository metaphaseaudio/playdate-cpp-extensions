//
// Created by Matt on 11/18/2023.
//

#include "pdcpp/graphics/ScopedGraphicsContext.h"
#include "pdcpp/components/Viewport.h"
#include <pdcpp/core/GlobalPlaydateAPI.h>


pdcpp::Viewport::Viewport()
    : m_OffsetX(0)
    , m_OffsetY(0)
{}

pdcpp::Viewport::Viewport(pdcpp::Component* content)
    : p_Content(content)
    , m_OffsetX(0)
    , m_OffsetY(0)
{}

void pdcpp::Viewport::setContent(pdcpp::Component* content) { p_Content = content; }

void pdcpp::Viewport::draw()
{
    if (p_Content == nullptr) { return; }

    pdcpp::Image img(0,0);
    {
        pdcpp::ScopedGraphicsContext context(p_Content->getBounds(), kColorClear, false);
        p_Content->redraw();
        img = context.getCopyAsImage();
    }

    const auto bounds = getBounds();
    pdcpp::GlobalPlaydateAPI::get()->graphics->setClipRect(bounds.x, bounds.y, bounds.width, bounds.height);
    img.draw({int(bounds.x) + m_OffsetX, int(bounds.y) + m_OffsetY});
    pdcpp::GlobalPlaydateAPI::get()->graphics->clearClipRect();
}

void pdcpp::Viewport::moveContentBy(int x, int y)
{
    m_OffsetX += x;
    m_OffsetY += y;
    redraw();
}

void pdcpp::Viewport::setContentOffset(int x, int y)
{
    m_OffsetX = x;
    m_OffsetY = y;
    redraw();
}

pdcpp::Point<int> pdcpp::Viewport::getContentOffset() const { return {m_OffsetX, m_OffsetY}; }

void pdcpp::Viewport::setViewPosition(int x, int y)
{
    m_OffsetX = -x;
    m_OffsetY = -y;
    redraw();
}

pdcpp::Point<int> pdcpp::Viewport::getViewPosition() const { return {-m_OffsetX, -m_OffsetY}; }
