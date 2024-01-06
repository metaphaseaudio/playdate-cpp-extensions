/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/27/2023
 *  Original author: MrBZapp
 */

#include "pdcpp/graphics/Colors.h"

LCDPattern transparent50GrayAPattern = {
        // Bitmap
        0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101,
        // Mask
        0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010,
};

LCDPattern transparent50GrayBPattern = {
        // Bitmap
        0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010,
        // Mask
        0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101,
};

LCDPattern solid50GrayAPattern = {
        // Bitmap
        0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern solid50GrayBPattern = {
    // Bitmap
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    // Mask
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern sparseCheckerAPattern = {
        // Bitmap
        0b11111111,
        0b11101110,
        0b11111111,
        0b10111011,
        0b11111111,
        0b11101110,
        0b11111111,
        0b10111011,

        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern crossHatchAPattern = {
        // Bitmap
        0b11101110, 0b10111011, 0b11011101, 0b01110111, 0b11101110, 0b10111011, 0b11011101, 0b01110111,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};


LCDPattern steppedDitherPattern0 = {
        // Bitmap
        0b10000000, 0b00010000, 0b00000010, 0b01000000, 0b00001000, 0b00000001, 0b00100000, 0b00000100,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern steppedDitherPattern1 = {
        // Bitmap
        0b11000000,
        0b00011000,
        0b00000011,
        0b01100000,
        0b00001100,
        0b10000001,
        0b00110000,
        0b00000110,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern steppedDitherPattern2 = {
        // Bitmap
        0b11000001, 0b00111000, 0b00000111, 0b11100000, 0b00011100, 0b10000011, 0b01110000, 0b00001110,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern steppedDitherPattern3 = {
        // Bitmap
        0b11100001, 0b00111100, 0b10000111, 0b11110000, 0b00011110, 0b11000011, 0b01111000, 0b00001111,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern steppedDitherPattern4 = {
        // Bitmap
        0b11100011, 0b01111100, 0b10001111, 0b11110001, 0b00111110, 0b11000111, 0b11111000, 0b00011111,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern steppedDitherPattern5 = {
        // Bitmap
        0b11110011, 0b01111110, 0b11001111, 0b11111001, 0b00111111, 0b11100111, 0b11111100, 0b10011111,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDPattern steppedDitherPattern6 = {
        // Bitmap
        0b11110111, 0b11111110, 0b11011111, 0b11111011, 0b01111111, 0b11101111, 0b11111101, 0b10111111,
        // Mask
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
};

LCDColor pdcpp::Colors::transparent50GrayA = reinterpret_cast<LCDColor>(transparent50GrayAPattern);
LCDColor pdcpp::Colors::transparent50GrayB = reinterpret_cast<LCDColor>(transparent50GrayBPattern);

LCDColor pdcpp::Colors::solid50GrayA = reinterpret_cast<LCDColor>(solid50GrayAPattern);
LCDColor pdcpp::Colors::solid50GrayB = reinterpret_cast<LCDColor>(solid50GrayBPattern);

LCDColor pdcpp::Colors::sparseCheckerA = reinterpret_cast<LCDColor>(sparseCheckerAPattern);

LCDColor pdcpp::Colors::steppedDither0 = reinterpret_cast<LCDColor>(steppedDitherPattern0);
LCDColor pdcpp::Colors::steppedDither1 = reinterpret_cast<LCDColor>(steppedDitherPattern1);
LCDColor pdcpp::Colors::steppedDither2 = reinterpret_cast<LCDColor>(steppedDitherPattern2);
LCDColor pdcpp::Colors::steppedDither3 = reinterpret_cast<LCDColor>(steppedDitherPattern3);
LCDColor pdcpp::Colors::steppedDither4 = reinterpret_cast<LCDColor>(steppedDitherPattern4);
LCDColor pdcpp::Colors::steppedDither5 = reinterpret_cast<LCDColor>(steppedDitherPattern5);
LCDColor pdcpp::Colors::steppedDither6 = reinterpret_cast<LCDColor>(steppedDitherPattern6);


LCDColor pdcpp::Colors::gradient[9] = {
    kColorBlack,
    steppedDither0,
    steppedDither1,
    steppedDither2,
    steppedDither3,
    steppedDither4,
    steppedDither5,
    steppedDither6,
    kColorWhite
};