#pragma once

#include "common.h"

namespace meta
{
    // index_list
    template <size_t... Idx> using index_list = std::index_sequence<Idx...>;
    template <size_t N> using make_index_list = std::make_index_sequence<N>;

    // type_list
    template <typename... Ts> struct type_list { static constexpr size_t size = sizeof...(Ts); };

    // to_type_list
    template <typename IList> struct to_type_list {};
    template <size_t... Idx> struct to_type_list<index_list<Idx...>> { using type = type_list<size_t_t<Idx>...>; };
    template <typename IList> using to_type_list_t = typename to_type_list<IList>::type;

    // to_index_list
    template <typename List> struct to_index_list {};
    template <size_t... Idx> struct to_index_list<type_list<size_t_t<Idx>...>> { using type = index_list<Idx...>; };
    template <typename List> using to_index_list_t = typename to_index_list<List>::type;

    // is_type_list
    template <typename T> struct is_type_list : false_t {};
    template <typename... Ts> struct is_type_list<type_list<Ts...>> : true_t {};
    template <typename T> constexpr bool is_type_list_v = is_type_list<T>::value;

    // concat
    template <typename K, typename L> struct concat {};
    template <typename... Ts, typename... Us>
    struct concat<type_list<Ts...>, type_list<Us...>> { using type = type_list<Ts..., Us...>; };
    template <typename K, typename L> using concat_t = typename concat<K, L>::type;

    // extract_list
    template <typename T> struct extract_list {};
    template <template <typename...> typename Templ, typename... Ts>
    struct extract_list<Templ<Ts...>> { using type = type_list<Ts...>; };
    template <typename T> using extract_list_t = typename extract_list<T>::type;

    // front
    template <typename List> struct front {};
    template <typename T, typename... Rest> struct front<type_list<T, Rest...>> { using type = T; };
    template <typename List> using front_t = typename front<List>::type;

    // nth_type
    namespace detail
    {
        template <typename List, typename IList, size_t N> struct nth_type_impl {};
        template <typename... Ts, size_t... Idx, size_t N>
        struct nth_type_impl<type_list<Ts...>, index_list<Idx...>, N> : indexed_type<Idx, Ts>... {};
        template <size_t N, typename T> T nth_type_selector(const indexed_type<N, T>&);
    }
    template <typename List, size_t N> struct nth_type {};
    template <typename... Ts, size_t N>
    struct nth_type<type_list<Ts...>, N>
    {
        using type = decltype(detail::nth_type_selector<N>(
            detail::nth_type_impl<type_list<Ts...>, make_index_list<sizeof...(Ts)>, N>{}));
    };
    template <typename List, size_t N> using nth_type_t = typename nth_type<List, N>::type;

    // nth_types
    template <typename List, typename IList> struct nth_types {};
    template <typename List, size_t... Idx>
    struct nth_types<List, index_list<Idx...>> { using type = type_list<nth_type_t<List, Idx>...>; };
    template <typename List, typename IList> using nth_types_t = typename nth_types<List, IList>::type;
}
