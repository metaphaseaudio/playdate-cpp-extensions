/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/30/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include <pdcpp/core/util.h>

namespace pdcpp
{
    class CollisionInfo
    {
    public:
        /**
         * RIIA wrapper for the collision info returned by `Sprite` methods
         * like `moveWithCollisions`, `checkCollisions`, etc.
         *
         * @param info The pointer returned by the underlying C API. This may
         *             be a null pointer if no collisions occurred.
         * @param x The resulting X position of the center of the Sprite.
         * @param y The resulting Y position of the center of the Sprite.
         * @param nCollisions The number of collisions that occurred.
         */
        CollisionInfo(::SpriteCollisionInfo* info, float x, float y, int nCollisions);

        // Move constructor
        CollisionInfo(CollisionInfo&& other) noexcept;

        // Move assignment constructor
        CollisionInfo& operator=(CollisionInfo&& other) noexcept;

        // Destructor. Frees the contained SpriteCollisionInfo data.
        ~CollisionInfo();

        /**
         * Returns the collision info at a given index. Check
         * `getNCollisions` first before calling this function, otherwise
         *  you will crash attempting to dereference a null pointer.
         *
         * @param i The element of the collision info to return
         * @return The `SpriteCollisionInfo` struct at position `i`
         */
        [[ nodiscard ]] const SpriteCollisionInfo& operator[](int i) const { return p_Info[i]; }

        /**
         * @return The destination X coordinate of the center of the sprite.
         */
        [[ nodiscard ]] float getX() const { return m_X; }

        /**
         * @return The destination Y coordinate of the center of the sprite.
         */
        [[ nodiscard ]] float getY() const { return m_Y; }

        /**
         * @return The number of collisions in this object
         */
        [[ nodiscard ]] int getNCollisions() const { return m_N; }

    private:
        float m_X, m_Y;
        int m_N;
        SpriteCollisionInfo* p_Info;

        PDCPP_DECLARE_NON_COPYABLE(CollisionInfo);
    };
}
