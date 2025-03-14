//
// Created by Matt on 3/2/2024.
//

#include "pdcpp/graphics/Blur.h"

pdcpp::Image pdcpp::gaussianBlur(const pdcpp::Image& img, int pxRadius)
{
    auto bitmapData = img.getBitmapData();
    return pdcpp::Image(bitmapData.bounds.width + pxRadius, bitmapData.bounds.height + pxRadius);
}
