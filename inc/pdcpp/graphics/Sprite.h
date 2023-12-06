/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/24/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <string>
#include <memory>
#include <functional>
#include <pd_api.h>
#include <pdcpp/core/util.h>
#include <pdcpp/graphics/Rectangle.h>

#include "CollisionInfo.h"
#include "Image.h"
#include "Point.h"

namespace pdcpp
{
    class Sprite {
    public:
        /**
         * Make a new sprite
         *
         * @param tag the optional tag to use. 0 by default.
         */
        explicit Sprite(uint8_t tag=0);

        /**
         * Make a new sprite using a `pdcpp::Image` (`draw` will not be called)
         *
         * @param img the image to use for this sprite
         * @param flip optionally flip the image around the x or y axis, or both
         *     default is un-flipped.
         * @param tag optionally tag this sprite. 0 by default.
         */
        explicit Sprite(const Image& img, LCDBitmapFlip flip=kBitmapUnflipped, uint8_t tag=0);

        /**
         * Make a new sprite using a raw LCDBitmap pointer (`draw` will not be
         * called)
         *
         * @param img the image to use for this sprite
         * @param flip optionally flip the image around the x or y axis, or both
         *     default is un-flipped.
         * @param tag optionally tag this sprite. 0 by default.
         */
        explicit Sprite(LCDBitmap* img, LCDBitmapFlip flip=kBitmapUnflipped, uint8_t tag=0);

        // Move constructor
        Sprite(Sprite&& other) noexcept;

        // Move-assignment constructor
        Sprite& operator=(Sprite&& rhs) noexcept;

        // Destructor. Removes and frees the sprite.
        ~Sprite();

        // Adds the sprite to the system's display lists. Enabled by default
        void addSprite();

        // Removes the sprite from the system's display list.
        void removeSprite();

        /**
         * Tell the system whether or not to bother calling this sprite's draw
         * function.All Sprites are visible by default.
         *
         * @param shouldBeVisible true makes enables drawing, false disables it.
         */
        void setVisible(bool shouldBeVisible) const;

        /**
         * Attempts to move the center of the sprite to the X/Y target. If a
         * collision occurs the sprite's `handleCollision` method will be called
         * to determine if how to handle the collision. The collision details
         * will be returned, including the resulting position of the sprite.
         *
         * @param targetX the x coordinate to which the sprite should be moved
         * @param targetY the y coordinate to which the sprite should be moved
         * @returns a CollisionInfo object listing the collisions (if any) and
         *     the resulting position of the sprite (which may not be the same
         *     as the target.)
         */
        CollisionInfo moveWithCollisions(float targetX, float targetY);

        /**
         * Same as `moveWithCollisions`, but does not actually move the sprite.
         *
         * @param targetX the x coordinate to which the sprite could be moved
         * @param targetY the y coordinate to which the sprite could be moved
         * @returns a CollisionInfo object listing the collisions (if any) and
         *     the resulting position of the sprite (which may not be the same
         *     as the target.)
         */
        CollisionInfo checkCollisions(float targetX, float targetY);

        /**
         * Same as `moveWithCollisions`, but using deltas instead of absolute
         * coordinates.
         *
         * @param deltaX the number of pixels to attempt to move the sprite
         *     along the X axis
         * @param deltaY the number of pixels to attempt to move the sprite
         *     along the Y axis.
         * @returns a CollisionInfo object listing the collisions (if any) and
         *     the resulting position of the sprite (which may not be the same
         *     as the target.)
         */
        CollisionInfo moveByWithCollisions(float deltaX, float deltaY);

        /**
         * Same as `moveByWithCollisions`, but does not actually move the Sprite
         *
         * @param deltaX the number of pixels to attempt to move the sprite
         *     along the X axis
         * @param deltaY the number of pixels to attempt to move the sprite
         *     along the Y axis.
         * @returns a CollisionInfo object listing the collisions (if any) and
         *     the resulting position of the sprite (which may not be the same
         *     as the target.)
         */
        CollisionInfo checkRelativeCollisions(float targetX, float targetY);

        /**
         * Enables/Disables collisions for this sprite. Default is enabled, but
         * doesn't actually matter unless the Collide Rect is set.
         */
        void setCollisionsEnabled(bool enable);

        /**
         * Marks the area of the given sprite, relative to its bounds, to be
         * checked for collisions with other sprites' collide rects
         *
         * @param bounds a const pdcpp::Rectangle<float>& for the bounds of the collisions
         */
        void setCollideRect(const pdcpp::Rectangle<float>& bounds);

        /**
         * @returns the current collision rectangle bounds relative to the
         *     Sprite.
         */
        [[ nodiscard ]] pdcpp::Rectangle<float> getCollideBounds() const;


        /**
         * @returns the current collision rectangle bounds relative to the
         *     world.
         */
        [[ nodiscard ]] pdcpp::Rectangle<float> getAbsoluteCollideBounds() const;


        //Removes the collision rectangle from this sprite.
        void clearCollideRect();

        /**
         * Moves the given sprite to x, y and resets its bounds based on the
         * bitmap dimensions and center
         *
         * @param x the X coordinate
         * @param y the Y coordinate
         */
        void moveTo(float x, float y);

        /**
         * Moves the given sprite to by offsetting its current position by X/Y
         * pixels (doesn't have to be whole pixels)
         *
         * @param x the X delta
         * @param y the Y delta
         */
        void moveBy(float x, float y);

        /**
         * Sets the Z order of the given sprite. Higher Z sprites are drawn on
         * top of those with lower Z order.
         *
         * @param index the target Z index
         */
        void setZIndex(int16_t index);

         // returns the Z index of this sprite
        [[ nodiscard ]] int16_t getZIndex() const;

        /**
         * Sets the the bounds of the sprite
         *
         * @param bounds a const pdcpp::Rectangle<float>& for the position, width, and height of the
         *     Sprite
         */
        void setBounds(const pdcpp::Rectangle<float>& bounds);

        /**
         * @returns a const pdcpp::Rectangle<float>& representing the current bounds of the sprite
         */
        [[ nodiscard ]] pdcpp::Rectangle<float> getBounds() const;

        /**
         * @returns a Point<float> of the position of the sprite.
         */
        [[ nodiscard ]] Point<float> getPosition() const;

        /**
         * Set the size of this sprite for drawing and when moving.
         *
         * @param width the new width of the sprite
         * @param height the new height of the sprite
         */
        void setSize(float width, float height);

        /**
         * Sets an image for this Sprite to draw instead of using the `draw`
         * method.
         *
         * @param img the image to draw
         * @param flip an optional axis (or two) around which to flip the image.
         *     un-flipped by default.
         */
        void setImage(const pdcpp::Image& img, LCDBitmapFlip flip=kBitmapUnflipped);

        /**
         * Sets an image for this Sprite to draw instead of using the `draw`
         * method using a raw LCDBitmap pointer. Useful for working with
         * `ImageTables` for animations.
         *
         * @param img the image to draw
         * @param flip an optional axis (or two) around which to flip the image.
         *     un-flipped by default.
         */
        void setImage(LCDBitmap* img, LCDBitmapFlip flip);

        /**
         * @returns a `Point<float>` representing the center of the Sprite.
         */
        [[ nodiscard ]] Point<float> getCenter() const;

        /**
         * Indicate that this sprite needs to be re-drawn. Many of the other
         * methods such as setting size, moving, changing image, etc. will all
         * mark the Sprite as dirty automatically, so this can frequently be
         * avoided, but there are cases where a sprite will need to be re-drawn
         * where there is no other way for the system to know.
         */
        void markDirty() const;

        /**
         * Indicate an area of this sprite which needs to be re-drawn.
         *
         * @param dirtyArea the bounds of the area to mark as dirty
         */
        void markAreaAsDirty(const pdcpp::Rectangle<float>& dirtyArea) const;

        /**
         * Gives this Sprite an identifiable. Very useful as a lightweight
         * substitute for runtime type information in scenarios where. Sprites
         * are tagged with a 0 by default.
         */
        void setTag(uint8_t);

        /**
         * @returns the tag set on this Sprite. 0 if un-set.
         */
        [[ nodiscard ]] uint8_t getTag() const;

        template <typename TagType>
        [[ nodiscard ]] TagType getTag() const { return static_cast<TagType>(getTag()); }

        /**
         * called whenever the Playdate API's `updateAndDrawSprites` method is
         * called, unless the updates for this sprite are disabled. Updates are
         * enabled, but do nothing by default. Inherit from `Sprite` and
         * override this method to do something particular like animations,
         * changing images, or changing shape.
         */
        virtual void update() {};

        /**
         * called whenever the Playdate API's `drawSprites` or
         * `updateAndDrawSprites` methods are called, unless the Sprite has an
         * `Image` set. Inherit from `Sprite` and override this method to
         * specify what should be drawn.
         *
         * @param bounds the bounds of the sprite. You can get this from
         *     `getBounds` as well, but it's likely you'll want this anyway, so
         *     can save you a little extra code.
         * @param drawrect the current dirty rect being updated by the display
         *     list.
         */
        virtual void redraw(const pdcpp::Rectangle<float>& bounds, const pdcpp::Rectangle<float>& drawrect) {};

        /**
         * When collisions are enabled on this sprite, this function will be
         * called when the system detects that a caller has either tried to move
         * this sprite, or requested information regarding potential collisions
         * if this sprite were to be moved, and the result brought the collision
         * bounds of this sprite into contact with the collision rectangle of
         * another sprite.
         *
         * It will be called once with every sprite with which this sprite
         * collided, or would have collided. Note that this is only ever called
         * on the Sprite initiating contact.
         *
         * By default, sprites will slide over each other. Inherit from `Sprite`
         * and use the Sprite tagging feature to specify different collision
         * response types for different types of sprites.
         *
         * @param other the Sprite with which this Sprite collided.
         * @returns a SpriteCollisionResponseType indicating how the system
         *     should respond to the collision.
         */
        virtual SpriteCollisionResponseType handleCollision(Sprite* other) { return kCollisionTypeOverlap; };

        /**
         * Called whenever the dimensions of this sprite have changed.
         */
        virtual void resized() {};

        /**
         * Attempts to retrieve the pointer to a raw Sprite from an LCDSprite's
         * User Data. Will return a nullptr if the LCDSprite does not resolve to
         * a pdcpp::Sprite.
         *
         * @param toCast a pointer to the LCDSprite to cast
         * @return a pointer to the LCDSprite's pdcpp::Sprite interface.
         */
        static Sprite* castSprite(LCDSprite* toCast);

        /**
         * Alias for the C API's sprite->updateAndRedraw so you don't have to
         * include the Global API header.
         */
         static void updateAndRedrawAllSprites();
    protected:
        LCDSprite* p_Sprite;
        PDCPP_DECLARE_NON_COPYABLE(Sprite);
    };
}
