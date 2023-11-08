/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/28/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <functional>
#include <pd_api.h>
#include <pdcpp/core/util.h>
#include "Image.h"

namespace pdcpp
{
    class ScopedGraphicsContext
    {
    public:
        /**
         * Manages pd graphics contexts in a way that allows for easy management
         * using syntax similar to thread mutexes. When declared, all calls to
         * the playdate C graphics API such as drawing mode will only effect the
         * calls to the current scope. This is very useful inside Sprite::draw
         * methods, where one may want to change modes without affecting every
         * other image, font, and sprite.
         *
         * The x/y position of any calls made while within the context will be
         * relative to the context. For example, if the bounds have a position
         * of 10, 10, anything drawn at position 0, 0, will draw at 10, 10.
         *
         * NOTE: The context will not be drawn to screen until *leaving* the
         * scope, so don't declare one of these at the top level of your game or
         * nothing will ever draw!
         *
         * @param bounds The size and placement of this context. Note that
         *     nothing can be drawn outside of the bounds, so size things
         *     accordingly.
         * @param bgColor fills the context with a color. default is clear.
         */
        explicit ScopedGraphicsContext(const PDRect& bounds, LCDColor bgColor=kColorClear, bool drawOnExit=true);

        /**
         * Copies the current state of the context's drawing buffer and returns
         * it for later use.
         *
         * @returns a copy of the current state of the drawing as a pdcpp::Image
         */
        [[ nodiscard ]] pdcpp::Image getCopyAsImage() const;

        // Destructor. leaves the scoped context.
        ~ScopedGraphicsContext();

    private:
        const PDRect& m_Bounds;
        LCDBitmap* m_Context;
        bool m_DrawOnExit;

        PDCPP_DECLARE_NON_COPYABLE_NON_MOVABLE(ScopedGraphicsContext);
    };
}
