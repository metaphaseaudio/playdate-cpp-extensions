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

constexpr float kAudioHardwareSampleRate = 44100.0f;

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
