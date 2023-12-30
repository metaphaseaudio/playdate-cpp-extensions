/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/24/2023
 *  Original author: MrBZapp
 */

#include <stdexcept>
#include <pdcpp/graphics/Image.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>
#include "pdcpp/graphics/ScopedGraphicsContext.h"


pdcpp::Image::Image(int width, int height, LCDColor bgColor)
    : p_Data(pdcpp::GlobalPlaydateAPI::get()->graphics->newBitmap(width, height, bgColor))
{}

pdcpp::Image::Image(const std::string& imgPath)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    const char* outErr = nullptr;
    p_Data = pd->graphics->loadBitmap(imgPath.c_str(), &outErr);

    if (outErr != nullptr)
        { pd->system->error("Error loading image: %s", std::string(outErr).c_str()); }
}

pdcpp::Image::Image(LCDBitmap* data)
    : p_Data(data)
{}

pdcpp::Image::~Image()
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    if (p_Data != nullptr)
        { pd->graphics->freeBitmap(p_Data); }
}

pdcpp::Image::Image(pdcpp::Image&& other) noexcept
    : p_Data(other.p_Data)
{ other.p_Data = nullptr; }

pdcpp::Image& pdcpp::Image::operator=(pdcpp::Image&& other) noexcept
{
    p_Data = other.p_Data;
    other.p_Data = nullptr;
    return *this;
}

pdcpp::Image::Image(const pdcpp::Image& other)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    p_Data = pd->graphics->copyBitmap(other.p_Data);
}

pdcpp::Image& pdcpp::Image::operator=(const pdcpp::Image& rhs)
{
    if (&rhs != this)
    {
        auto pd = pdcpp::GlobalPlaydateAPI::get();
        p_Data = pd->graphics->copyBitmap(rhs.p_Data);
    }

    return *this;
}

pdcpp::Image pdcpp::Image::copyFromPointer(LCDBitmap* toCopy)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    return Image(pd->graphics->copyBitmap(toCopy));
}

void pdcpp::Image::rotateAndScale(float angle, float xScale, float yScale)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    LCDBitmap* lastPtr = p_Data;
    int _;
    p_Data = pd->graphics->rotatedBitmap(p_Data, angle, xScale, yScale, &_);
    pd->graphics->freeBitmap(lastPtr);

}

pdcpp::Image pdcpp::Image::withRotationAndScale(float angle, float xScale, float yScale) const
{
    int _;
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    return pdcpp::Image(pd->graphics->rotatedBitmap(p_Data, angle, xScale, yScale, &_));
}

void pdcpp::Image::setMask(const pdcpp::Image& mask) { pdcpp::GlobalPlaydateAPI::get()->graphics->setBitmapMask(p_Data, mask); }
LCDBitmap* pdcpp::Image::getMask() const { return pdcpp::GlobalPlaydateAPI::get()->graphics->getBitmapMask(p_Data); }
void pdcpp::Image::clearMask() { pdcpp::GlobalPlaydateAPI::get()->graphics->setBitmapMask(p_Data, nullptr); }

void pdcpp::Image::draw(const pdcpp::Point<int>& location, LCDBitmapFlip flip) const
    { pdcpp::GlobalPlaydateAPI::get()->graphics->drawBitmap(p_Data, location.x, location.y, flip); }

void pdcpp::Image::draw(const pdcpp::Point<int>& location, float xScale, float yScale) const
    { pdcpp::GlobalPlaydateAPI::get()->graphics->drawScaledBitmap(p_Data, location.x, location.y, xScale, yScale); }


void pdcpp::Image::draw(const pdcpp::Point<int>& location, float degrees, float centerX, float centerY, float xScale, float yScale) const
{
    // TODO: This is to work around a bug where drawing is inverted around the
    //  centerY axis when the rotation angle is 90 or 180 degrees:
    //  https://devforum.play.date/t/drawrotatedbitmap-draws-from-wrong-center-if-degrees-is-one-of-the-four-cardinal-directions/3620/8
    //  remove it when the bug is actually fixed.
    if ((degrees) == 90.0f || ::fabs(degrees) == 180.0f)
        { centerY = 1.0f - centerY; }
    pdcpp::GlobalPlaydateAPI::get()->graphics->drawRotatedBitmap(p_Data, location.x, location.y, degrees, centerX, centerY, xScale, yScale);
}

void pdcpp::Image::fill(LCDColor color)
{
    pdcpp::GlobalPlaydateAPI::get()->graphics->clearBitmap(p_Data, color);
}

pdcpp::Image pdcpp::Image::drawAsImage(const PDRect& bounds, const std::function<void(const playdate_graphics*)>& drawFunc, LCDSolidColor fillColor)
{
    pdcpp::ScopedGraphicsContext context(bounds, fillColor, false);
    drawFunc(pdcpp::GlobalPlaydateAPI::get()->graphics);
    return context.getCopyAsImage();
}
