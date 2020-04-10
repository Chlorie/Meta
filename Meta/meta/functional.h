#pragma once

#include "common.h"

namespace meta
{
    // Arithmetic
    template <typename T, typename U> using plus = integral_t<T::value + U::value>;
    template <typename T, typename U> using minus = integral_t<T::value - U::value>;
    template <typename T, typename U> using multiplies = integral_t<T::value * U::value>;
    template <typename T, typename U> using divides = integral_t<T::value / U::value>;
    template <typename T, typename U> using modulus = integral_t<T::value % U::value>;
    template <typename T> using negate = integral_t<-T::value>;

    // Comparison
    template <typename T, typename U> using equal_to = integral_t<T::value == U::value>;
    template <typename T, typename U> using not_equal_to = integral_t<T::value != U::value>;
    template <typename T, typename U> using greater = integral_t<(T::value > U::value)>;
    template <typename T, typename U> using less = integral_t<(T::value < U::value)>;
    template <typename T, typename U> using greater_equal = integral_t<(T::value >= U::value)>;
    template <typename T, typename U> using less_equal = integral_t<(T::value <= U::value)>;

    // Logical
    template <typename T, typename U> using logical_and = integral_t<T::value && U::value>;
    template <typename T, typename U> using logical_or = integral_t<T::value || U::value>;
    template <typename T> using logical_not = integral_t<!T::value>;

    // Bitwise
    template <typename T, typename U> using bit_and = integral_t<T::value & U::value>;
    template <typename T, typename U> using bit_or = integral_t<T::value | U::value>;
    template <typename T, typename U> using bit_xor = integral_t<T::value ^ U::value>;
    template <typename T> using bit_not = integral_t<~T::value>;
}
