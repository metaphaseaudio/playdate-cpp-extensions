/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/25/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <algorithm>

constexpr float kAudioHardwareSampleRate = 44100.0f;
constexpr float kPI = 3.14159f;


/**
 * Use this macro from within a class definition to delete the copy and
 * assignment constructors for the given class.
 */
#define PDCPP_DECLARE_NON_COPYABLE(classname) \
    classname(const classname&) = delete;\
    classname& operator= (const classname&) = delete;

/**
 * Use this macro from within a class definition to delete the copy, move, and
 * assignment constructors for the given class.
 */
#define PDCPP_DECLARE_NON_COPYABLE_NON_MOVABLE(classname) \
    PDCPP_DECLARE_NON_COPYABLE(classname); \
    classname(classname&&) = delete;

namespace pdcpp
{
    template<typename T>
    T limit(T min, T max, T val)
        { return std::min<T>(std::max<T>(val, min), max); }


    template <typename T=float>
    float degToRad(float deg) { return deg * kPI / 180.0f; }

    static int wrapIndex(int index, int max) { return ((index % max) + max) % max; };
}