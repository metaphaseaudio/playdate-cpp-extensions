/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/4/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <algorithm>
#include <pd_api.h>
#include <cassert>
#include "Point.h"


namespace pdcpp
{
    template <typename T>
    struct Rectangle
    {
        Rectangle() :x(0), y(0), width(0), height(0) {};

        Rectangle(T x, T y, T width, T height)
            : x(x), y(y), width(width), height(height)
        {
            assert(width >= 0);
            assert(height >= 0);
        };

        explicit Rectangle(const PDRect& rect)
            : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
        {}

        Rectangle(pdcpp::Point<T> upperLeft, pdcpp::Point<T> bottomRight)
            : x(upperLeft.x), y(upperLeft.y), width(bottomRight.x - upperLeft.x), height(bottomRight.y - upperLeft.y)
        {}

        [[ nodiscard ]] pdcpp::Point<T> getTopLeft() const;
        [[ nodiscard ]] pdcpp::Point<T> getTopRight() const;
        [[ nodiscard ]] pdcpp::Point<T> getBottomLeft() const;
        [[ nodiscard ]] pdcpp::Point<T> getBottomRight() const;
        [[ nodiscard ]] pdcpp::Point<T> getCenter() const;
        void setCenter(pdcpp::Point<T> center);
        
        [[ nodiscard ]] Rectangle<T> getOverlap(const Rectangle<T>& other) const;
        
        Rectangle<T> removeFromLeft(T amt);
        Rectangle<T> removeFromRight(T amt);
        Rectangle<T> removeFromTop(T amt);
        Rectangle<T> removeFromBottom(T amt);
        [[ nodiscard ]] Rectangle<T> reduced(T amt) const;
        [[ nodiscard ]] Rectangle<T> expanded(T amt) const;

        [[ nodiscard ]] Rectangle<T> withOrigin(const pdcpp::Point<T>& newOrigin) const;
        [[ nodiscard ]] Rectangle<T> withCenter(const pdcpp::Point<T>& newCenter) const;

        [[ nodiscard ]] Rectangle<T> withWidth(T newWidth) const;
        [[ nodiscard ]] Rectangle<T> withHeight(T newHeight) const;

        T getRight() { return x + width; }
        T getBottom() { return y + height; }

        template <typename T2>
        [[ nodiscard ]] Rectangle<T2> toType() const
            { return Rectangle<T2>(T2(x), T2(y), T2(width), T2(height)); };
        [[ nodiscard ]] Rectangle<int> toInt() const { return toType<int>(); };
        [[ nodiscard ]] Rectangle<float> toFloat() const { return toType<float>(); };

        [[ nodiscard ]] bool testPoint(const pdcpp::Point<T>& point) const;

        [[ nodiscard ]] operator PDRect() const;  // NOLINT (*-explicit-constructor)

        T x = 0, y = 0, width = 0, height = 0;
    };

    template<typename T>
    bool Rectangle<T>::testPoint(const Point<T>& point) const
    {
        return point.x > x && point.x < x + width && point.y > y && point.y < y + width;
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::withWidth(T newWidth) const
    {
        return Rectangle<T>(x, y, newWidth, height);
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::withHeight(T newHeight) const
    {
        return Rectangle<T>(x, y, width, newHeight);
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::withCenter(const Point<T>& newCenter) const
    {
        const auto currentCenter = getCenter();
        const auto diff = newCenter - currentCenter;
        return Rectangle<T>(x + diff.x, y + diff.y, width, height);
    }

    template<typename T>
    void Rectangle<T>::setCenter(Point<T> center)
    {
        x = center.x - (width / 2.0f);
        y = center.y - (height / 2.0f);
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::withOrigin(const Point<T>& newOrigin) const
        { return Rectangle<T>(newOrigin.x, newOrigin.y, width, height); }

    template<typename T>
    Rectangle<T> Rectangle<T>::reduced(T amt) const
    {
        return Rectangle<T>(x + amt, y + amt, std::max<T>(0, width - amt * 2), std::max<T>(0, height - amt * 2));
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::expanded(T amt) const
    {
        return Rectangle<T>(x - amt, y - amt, std::max<T>(0, width + amt * 2), std::max<T>(0, height + amt * 2));
    }

    template<typename T>
    Rectangle<T>::operator PDRect() const
        { return {float(x), float(y), float(width), float(height)}; }

    template<typename T>
    Point<T> Rectangle<T>::getTopLeft() const { return {x, y}; }

    template<typename T>
    Point<T> Rectangle<T>::getTopRight() const { return Point<T>(x + width, y); }

    template<typename T>
    Point<T> Rectangle<T>::getBottomLeft() const { return Point<T>(x, y + height); }

    template<typename T>
    Point<T> Rectangle<T>::getBottomRight() const { return {x + width, y + height}; }
    
    template<typename T>
    Point<T> Rectangle<T>::getCenter() const
        { return {x + T(width / 2.0f), y + T(height / 2.0f)}; }

    template<typename T>
    Rectangle<T> Rectangle<T>::getOverlap(const Rectangle<T>& other) const
    {
        const auto overlapStartX = std::max<T>(x, other.x);
        const auto overlapEndX = std::min<T>(other.x + other.width, x + width);
        const auto overlapLenX = std::max<T>(0.0f, overlapEndX - overlapStartX);

        const auto overlapStartY = std::max<T>(other.y, y);
        const auto overlapEndY = std::min<T>(other.y + other.height, y + height);
        const auto overlapLenY = std::max<T>(0.0f, overlapEndY - overlapStartY);

        return Rectangle<T>(
            overlapLenX > 0 ? overlapStartX : 0.0f,
            overlapLenY > 0 ? overlapStartY : 0.0f,
            overlapLenX,
            overlapLenY
        );
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromLeft(T amt)
    {
        amt = std::min<T>(width, amt);
        Rectangle<T> rv = {x, y, amt, height};
        x += amt;
        width -= amt;
        return rv;
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromRight(T amt)
    {
        amt = std::min<T>(width, amt);
        Rectangle<T> rv = {x + width - amt, y, amt, height};
        width -= amt;
        return rv;
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromTop(T amt)
    {
        amt = std::min<T>(height, amt);
        Rectangle<T> rv = {x, y, width, amt};
        y += amt;
        height -= amt;
        return rv;
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromBottom(T amt)
    {
        amt = std::min<T>(height, amt);
        Rectangle<T> rv = {x, y + height - amt, width, amt};
        height -= amt;
        return rv;
    }
}
