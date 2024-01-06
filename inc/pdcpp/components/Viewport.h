/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 11/18/2023
 *  Original author: MrBZapp
 */
#pragma once

#include "Component.h"
#include "pdcpp/graphics/Point.h"
#include "pdcpp/graphics/Sprite.h"
#include <pd_api.h>

namespace pdcpp
{
    class Viewport
        : public Component
    {
    public:
        /**
         * This component takes a pointer to a Component which is not a part of
         * its direct hierarchy, but provides a view of that component through
         * its own bounds, and handles to move the viewed component around so
         * users can see different parts.
         */
        Viewport();

        /**
         * Creates a Viewport with a viewed content set at construction time
         * @param content
         */
        explicit Viewport(Component* content);

        /**
         * Sets the content of the viewport
         *
         * @param content the Component to view
         */
        void setContent(Component* content);

        /**
         * Move the content relative to the content bounds. The Viewport will
         * place the X/Y coordinates of the content in the upper left hand
         * corner of the Viewport.
         *
         * @param x the x coordinate in the content to use as the upper left
         * @param y the y coordinate in the content to use as the upper left
         */
        void setViewPosition(int x, int y);

        /**
         * @returns the current position of the view relative to the content
         */
        [[ nodiscard ]] pdcpp::Point<int> getViewPosition() const;

        /**
         * Like `setViewPosition`, but from the reference point of the Viewport,
         * ie. inverted.
         *
         * @param x how much the content should be offset within the viewport on
         *     the x axis
         * @param y how much the content should be offset within the viewport on
         *     the y axis
         */
        void setContentOffset(int x, int y);

        /**
         * @returns the current offset of the content within the viewport
         */
        [[ nodiscard ]] pdcpp::Point<int> getContentOffset() const;

        /**
         * Move the content n pixes in the x and or y directions
         *
         * @param x the distance along the x axis to move the content
         * @param y the distance along the y axis to move the content
         */
        void moveContentBy(int x, int y);

        // Overrides base class method.
        void draw() override;

    private:
        Component* p_Content;
        int m_OffsetX, m_OffsetY;
    };
}
