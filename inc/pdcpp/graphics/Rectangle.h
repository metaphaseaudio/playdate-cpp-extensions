//
// Created by Matt on 12/4/2023.
//

#pragma once
#include <algorithm>
#include <pd_api.h>
#include "Point.h"


namespace pdcpp
{
    template <typename T>
    struct Rectangle
    {
        Rectangle(T x, T y, T width, T height)
            : x(x), y(y), width(width), height(height)
        {};

        explicit Rectangle(const PDRect& rect)
            : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
        {}

        pdcpp::Point<T> getBottomRight();
        [[ nodiscard ]] pdcpp::Point<T> getCenter() const;
        
        Rectangle<T> getOverlap(const Rectangle<T>& other);
        
        Rectangle<T> removeFromLeft(T amt);
        Rectangle<T> removeFromRight(T amt);
        Rectangle<T> removeFromTop(T amt);
        Rectangle<T> removeFromBottom(T amt);
        Rectangle<T> reduced(T amt) const;
        Rectangle<T> withOrigin(const pdcpp::Point<T>& newOrigin);

        T getRight() { x + width; }
        T getBottom() { y + height; }

        [[ nodiscard ]] operator PDRect() const;  // NOLINT (*-explicit-constructor)

        T x = 0, y = 0, width = 0, height = 0;
    };

    template<typename T>
    Rectangle<T> Rectangle<T>::withOrigin(const Point<T>& newOrigin)
        { return Rectangle<T>(newOrigin.x, newOrigin.y, width, height); }

    template<typename T>
    Rectangle<T> Rectangle<T>::reduced(T amt) const
    {
        return Rectangle<T>(x + amt, y + amt, width - amt * 2, height - amt * 2);
    }

    template<typename T>
    Rectangle<T>::operator PDRect() const
        { return {float(x), float(y), float(width), float(height)}; }

    template<typename T>
    Point<T> Rectangle<T>::getBottomRight() { return {x + width, y + height}; }
    
    template<typename T>
    Point<T> Rectangle<T>::getCenter() const
        { return {x + T(width / 2.0f), y + T(height / 2.0f)}; }

    template<typename T>
    Rectangle<T> Rectangle<T>::getOverlap(const Rectangle<T>& other)
    {
        const auto overlapStartX = std::max(x, other.x);
        const auto overlapEndX = std::min(other.x + other.width, x + width);
        const auto overlapLenX = std::max(0.0f, overlapEndX - overlapStartX);

        const auto overlapStartY = std::max(other.y, y);
        const auto overlapEndY = std::min(other.y + other.height, y + height);
        const auto overlapLenY = std::max(0.0f, overlapEndY - overlapStartY);

        return {
            overlapLenX > 0 ? overlapStartX : 0.0f,
            overlapLenY > 0 ? overlapStartY : 0.0f,
            overlapLenX,
            overlapLenY
        }; 
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromLeft(T amt)
    {
        Rectangle<T> rv = {x, y, amt, height};
        x += amt;
        width -= amt;
        return rv;
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromRight(T amt)
    {
        Rectangle<T> rv = {x + width - amt, y, amt, height};
        width -= amt;
        return rv;
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromTop(T amt)
    {
        Rectangle<T> rv = {x, y, width, amt};
        y += amt;
        height -= amt;
        return rv;
    }

    template<typename T>
    Rectangle<T> Rectangle<T>::removeFromBottom(T amt)
    {
        Rectangle<T> rv = {x, y + height - amt, width, amt};
        height -= amt;
        return rv;
    }
}
