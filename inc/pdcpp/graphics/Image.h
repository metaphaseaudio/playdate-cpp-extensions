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

#include <functional>
#include <string>
#include <pd_api.h>
#include <pdcpp/core/util.h>
#include "Point.h"
#include "Rectangle.h"

namespace pdcpp
{
    class Image
    {
    public:
        /**
         * Creates a new Image with a specific height and width, optionally
         * filled with a color.
         *
         * @param width the width of the image
         * @param height the height of the image
         * @param bgColor the fill color. default is clear.
         */
        Image(int width=0, int height=0, LCDColor bgColor=kColorClear);

        /**
         * Loads an image from a given path and manages its memory. Any issues
         * loading the path will generate system errors with logging. Don't
         * provide image suffixes, the Playdate system manages that stuff.
         *
         * @param imgPath the path to load as an image
         */
        explicit Image(const std::string& imgPath);

        Image(const pdcpp::Rectangle<int>& bounds, uint8_t* data, uint8_t* mask, int rowStride=-1);

        /**
         * Copy constructor. Will call the Playdate C API under the hood so be
         * very judicious with the use of this constructor. Prefer passing
         * references and pointers, and move assigment methods like `emplace` to
         * avoid unnecessary temporary allocations.
         *
         * @param other The image to copy
         */
        Image(const Image& other);

        /**
         * Copy-assignment constructor. Will call the Playdate C API under the
         * hood so be very judicious with the use of this constructor. Prefer
         * passing references and pointers, and move assigment methods like
         * `emplace` to avoid unnecessary temporary allocations.
         *
         * @param other The image to copy-assign
         */
        Image& operator= (const Image&);

        // Move constructor
        Image(Image&& other) noexcept;

        // Move-assignment constructor
        Image& operator=(Image&& other) noexcept;

        /**
         * Explicitly copies a raw LCDBitmap pointer, returning an `Image` which
         * owns the copied data, and *not* the raw pointer. This is for objects
         * like `ImageTable` which retain ownership of their `LCDBitmap` memory,
         * but where the caller needs a copy of a single frame for one reason or
         * another. This is the only public API for creating an `Image` from a
         * raw `LCDBitmap` pointer to remove the risk of leaks or double-freeing
         * the raw pointer.
         *
         * @param toCopy a raw LCDBitmap pointer to copy
         * @return an Image of the copy
         */
        static Image copyFromPointer(LCDBitmap* toCopy);

        // Destructor. Frees the image data.
        ~Image();

        /**
         * Clears the bitmap, filling it with the given color.
         * @param color the LCDColor struct to use for filling
         */
        void fill(LCDColor color);

        /**
         * Rotates/Scales this image in place.
         *
         * @param angle the angle in degrees to rotate this image positive for
         *     clockwise, negative for counter clockwise
         * @param xScale a factor by which the size of the width of the image
         *     should be multiplied (ie. 0.5f is half size, 2.0f is twice the
         *     size, 1.0f results in no change)
         * @param yScale a factor by which the size of the height of the image
         *     should be multiplied (ie. 0.5f is half size, 2.0f is twice the
         *     size, 1.0f results in no change)
         */
        void rotateAndScale(float angle, float xScale, float yScale);

        /**
         * Does the same operation as `rotateAndScale`, but leaves this object
         * untouched and returns a new `Image` object with the applied rotation
         * and/or scale.
         *
         * @param angle the angle in degrees to rotate this image positive for
         *     clockwise, negative for counter clockwise
         * @param xScale a factor by which the size of the width of the image
         *     should be multiplied (ie. 0.5f is half size, 2.0f is twice the
         *     size, 1.0f results in no change)
         * @param yScale a factor by which the size of the height of the image
         *     should be multiplied (ie. 0.5f is half size, 2.0f is twice the
         *     size, 1.0f results in no change)
         */
        [[ nodiscard]] Image withRotationAndScale(float angle, float xScale, float yScale) const;

        /**
         * Sets a mask for the image. The mask must be the same size as the
         * image.
         *
         * @param mask an image to use as the mask
         */
        void setMask(const Image& mask);

        /**
         * Clears any masks set on this image. Idempotent.
         */
        void clearMask();

        /**
         * Returns the current mask for the image in the form of a raw LCDBitmap
         * pointer. May return a null pointer if no mask is set.
         *
         * @return a raw LCDBitmap pointer of the mask currently set on this
         *     image
         */
        [[ nodiscard ]] LCDBitmap* getMask() const;

        /**
         * @returns the bounds of the image with a 0, 0 origin point.
         */
        [[ nodiscard ]] pdcpp::Rectangle<int> getBounds() const;

        /**
         * Describes an image as a Rect of bounds, and an array of unsigned
         * 8-bit ints, one for the mask, and one for the actual data.
         */
        struct RawBitmapData
        {
            pdcpp::Rectangle<int> bounds;
            std::vector<uint8_t> mask;
            std::vector<uint8_t> data;
        };

        /**
         * @returns a structure to access the bitmap data of this image directly
         */
        [[ nodiscard ]] RawBitmapData getBitmapData() const;

        /**
         * Gets the color of the pixel at (x, y).
         *
         * @param x the x coordinate
         * @param y the y coordinate
         * @return If the coordinate is outside
         *      the bounds of the bitmap, or if the bitmap has a mask and the pixel
         *      is marked transparent, the function returns kColorClear. Otherwise
         *      the return is kColorWhite or kColorBlack;
         */
        [[ nodiscard ]] LCDSolidColor getPixel(int x, int y) const;

        /**
         * Draws the image at a given point. Optionally flip the image around
         * its center point. Does not modify the image
         *
         * @param location the location to draw the image.
         * @param flip the optional flip to apply before drawing
         */
        void draw(const Point<int>& location, LCDBitmapFlip flip=kBitmapUnflipped) const;

        /**
         * Draws the image at a given point with an alteration to the scale of
         * the width, height, or both. Negative scales correspond with flips.
         * Does not modify the image.
         *
         * @param location the upper right corner of the image to draw
         * @param xScale the scale factor to apply to the width
         * @param yScale the scale factor to apply to the height
         */
        void draw(const Point<int>& location, float xScale, float yScale) const;

        /**
         * Draws the image at a given point with a rotation around a specific
         * center point, and an optional alteration to the scale of the width,
         * height, or both. Negative scales correspond with flips. Does not
         * modify the image.
         *
         * @param location the center of the image to draw
         * @param centerX the ratio of the image's width at which to place the
         *     center point around which to rotate. Default is 0.5f, the center
         *     of the image's width.
         * @param centerY the ratio of the image's height at which to place the
         *     center point around which to rotate. Default is 0.5f, the center
         *     of the image's height.
         * @param degrees the rotation to apply to the image, positive for
         *     clockwise rotation, negative for counter-clockwise rotation.
         * @param xScale the optional scale factor to apply to the width
         * @param yScale the optional scale factor to apply to the height
         */
        void draw(const Point<int>& location, float degrees, float centerX=0.5f, float centerY=0.5f, float xScale=1.0f, float yScale=1.0f) const;

        /**
         * Use the playdate graphics API to draw an image
         *
         * @param bounds the bounds (ignoring the origin) of the image
         * @param drawFunc the function which will be used to draw the image
         * @param fillColor optional background color with which the image will
         *     be filled before drawing.
         * @return the drawn image
         */
        static Image drawAsImage
            (const PDRect& bounds, const std::function<void(const playdate_graphics*)>& drawFunc, LCDSolidColor fillColor=kColorClear);

        /**
         * Provides access to the underlying LCDBitmap for use with the C API.
         * This is important for Sprites and some of the `Image` methods which
         * require access to the memory, but cannot copy or own the memory such
         * as `setMask`. **Do NOT call `freeBitmap` on this pointer!**
         *
         * Implicit conversion is permitted on this object so it's possible to
         * pass this object directly to any C function taking a pointer to an
         * LCDBitmap structure.
         *
         * @return the LCDBitmap* owned by this Image
         */
        [[ nodiscard ]] operator LCDBitmap*() const { return p_Data; }; // NOLINT(*-explicit-constructor)

    private:
        /**
         * Private constructor for managing images directly from raw pointers.
         * The resulting image will own the pointer provided. Doing this
         * outside of this class opens the door for memory leaks or pointer
         * duplication/double-free-ing, so this is only ever used internally.
         *
         * @param data the LCDBitmap pointer of which to take ownership
         */
        explicit Image(LCDBitmap* data);

        LCDBitmap* p_Data;
    };
}
