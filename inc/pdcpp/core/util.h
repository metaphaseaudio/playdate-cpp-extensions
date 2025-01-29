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
#include <optional>
#include <sstream>

constexpr float kAudioHardwareSampleRate = 44100.0f;
constexpr float kPI = 3.14159f;


/**
 * Use this macro from within a class definition to delete the copy and
 * copy-assignment constructors for the given class.
 */
#define PDCPP_DECLARE_NON_COPYABLE(classname) \
    classname(const classname&) = delete; \
    classname& operator= (classname&) = delete;

/**
 * Use this macro from within a class definition to delete move, and
 * move-assignment constructors for the given class.
 */
#define PDCPP_DECLARE_NON_MOVABLE(classname) \
    classname(classname&&) = delete; \
    classname& operator= (classname&&) = delete;

/**
 * Use this macro from within a class definition to delete the copy,
 * copy-assignment, move, and move-assignment constructors for the given class.
 */
#define PDCPP_DECLARE_NON_COPYABLE_NON_MOVABLE(classname) \
    PDCPP_DECLARE_NON_COPYABLE(classname) \
    PDCPP_DECLARE_NON_MOVABLE(classname)


namespace pdcpp
{
    /**
     * Ensures a value stays between a minimum and maximum.
     *
     * @tparam T The type of the parameter.
     * @param min The minimum of the range in which to limit val
     * @param max The maximum of the range in which to limit val
     * @param val The value to limit
     * @return Either the val, min, or max, depending if val falls within the
     *     limit, and where it falls outside of it.
     */
    template<typename T>
    T limit(T min, T max, T val)
        { return std::min<T>(std::max<T>(val, min < max ? min : max), max > min ? max : min); }


    /**
     * Convert degrees to radians. Most easy math for circles/audio is in
     * radians, but it's sometimes easier to think in degrees, and the Playdate
     * C API is almost exclusively in degrees too.
     *
     * @tparam T The type of the input
     * @param deg the degrees to convert
     * @return the radian value
     */
    template <typename T=float>
    float degToRad(float deg) { return deg * kPI / 180.0f; }

    /**
     * Branch-less wrap an index in both directions 0 to index % max.
     *
     * @param index the index to wrap
     * @param max the max index by which to modulo
     * @return the wrapped index
     */
    [[maybe_unused]] static int wrapIndex(int index, int max) { return ((index % max) + max) % max; };

    template <typename T>
    std::string to_string_with_precision(const T a_value, const int n = 6)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return std::move(out).str();
    }


    /**
     * Template class that allows std::visit to take a group of lambda functions
     *
     * To use:
     * ```
     * std::variant<int, float, std::string> myVariant;
     *
     * std::visit(
     *     Overload
     *     {
     *         [](int val) { ... },
     *         [](std::string& val) { ... },
     *         [](float val) { ... },
     *     },
     *     myVariant);
     * ```
     */
    template<typename... Ts>
    struct Overload : Ts... {
        using Ts::operator()...;
    };
    template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

    template<typename T, typename X>
    bool isInstance(X* x)
    {
        return dynamic_cast<const T*>(x) != nullptr;
    }

    /**
     * stride function for use with std::views::filter to enable stride_view-ish
     * behaviours in C++20.
     *
     * usage:
     *      view | std::views::filter(stride(2)); // takes every other element
     */
    static constexpr auto stride =  [](int n)
    {
        return [s=-1, n](auto const&) mutable { s = (s + 1) % n; return !s; };
    };

    /**
     * Allows specification of std::ranges::range in function arguments.
     *
     * usage:
     *      void specialized(RangeOf<int> ints) { ... }; // Function only accepts ranges of integers
     *
     * @tparam R
     * @tparam V
     */
    template <typename R, typename V>
    concept RangeOf = std::ranges::range<R> && std::convertible_to<std::ranges::range_value_t<R>, V>;


    static double gainToDB(double gain) { return 20 * std::log10(gain); }
    static double dbToGain(double db) { return std::pow(10.0, db / 20.0); }

    template <typename T>
    std::tuple<int, T> make_integral_and_fractional(T value)
    {
        auto i = static_cast<int>(value);
        auto f = value - i;
        return {i, f};
    }

    template <typename T>
    static inline T linear_interpolate(T a, T b, float weight) { return (a * (1 - weight)) + (b * weight); }
}