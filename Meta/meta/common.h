#pragma once

#include <type_traits>

namespace meta
{
    using false_t = std::false_type;
    using true_t = std::true_type;
    template <bool B> using bool_t = std::bool_constant<B>;
    template <size_t S> using size_t_t = std::integral_constant<size_t, S>;
    template <auto V> using integral_t = std::integral_constant<decltype(V), V>;
    template <typename T> struct identity { using type = T; };
    template <typename T> using identity_t = typename identity<T>::type;
    template <size_t I, typename T> struct indexed_type
    {
        static constexpr size_t index = I;
        using type = T;
    };
}
