//
// Created by Matt on 9/27/2023.
//

#include "pdcpp/graphics/Colours.h"

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

LCDColor pdcpp::Colours::transparent50GrayA = reinterpret_cast<LCDColor>(transparent50GrayAPattern);
LCDColor pdcpp::Colours::transparent50GrayB = reinterpret_cast<LCDColor>(transparent50GrayBPattern);

LCDColor pdcpp::Colours::solid50GrayA = reinterpret_cast<LCDColor>(solid50GrayAPattern);
LCDColor pdcpp::Colours::solid50GrayB = reinterpret_cast<LCDColor>(solid50GrayBPattern);

LCDColor pdcpp::Colours::sparseCheckerA = reinterpret_cast<LCDColor>(sparseCheckerAPattern);