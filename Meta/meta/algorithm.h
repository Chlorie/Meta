#pragma once

#include "list.h"

namespace meta
{
    // apply_template
    template <typename List, template <typename...> typename Templ> struct apply_template {};
    template <typename... Ts, template <typename...> typename Templ>
    struct apply_template<type_list<Ts...>, Templ> { using type = Templ<Ts...>; };
    template <typename List, template <typename...> typename Templ>
    using apply_template_t = typename apply_template<List, Templ>::type;

    // size_of
    template <typename List> struct size_of {};
    template <typename... Ts> struct size_of<type_list<Ts...>> : size_t_t<sizeof...(Ts)> {};
    template <typename List> constexpr size_t size_of_v = size_of<List>::value;

    // is_empty
    template <typename T> constexpr bool is_empty_v = size_of_v<T> == 0;
    template <typename T> struct is_empty : bool_t<is_empty_v<T>> {};

    // reduce
    template <typename List, template <typename, typename> typename Reducer> struct reduce {};
    template <typename T, typename U, typename... Rest, template <typename, typename> typename Reducer>
    struct reduce<type_list<T, U, Rest...>, Reducer>
    {
        using type = typename reduce<
            type_list<Reducer<T, U>, Rest...>, Reducer>::type; // Recursion
    };
    template <typename T, template <typename, typename> typename Reducer>
    struct reduce<type_list<T>, Reducer> { using type = T; }; // End of recursion
    template <typename List, template <typename, typename> typename Reducer>
    using reduce_t = typename reduce<List, Reducer>::type;

    // all_of
    template <typename List, template <typename> typename Predicate> struct all_of : false_t {};
    template <typename... Ts, template <typename> typename Predicate>
    struct all_of<type_list<Ts...>, Predicate> : bool_t<(Predicate<Ts>::value&& ...)> {};
    template <typename List, template <typename> typename Predicate>
    constexpr bool all_of_v = all_of<List, Predicate>::value;

    // any_of
    template <typename List, template <typename> typename Predicate> struct any_of : true_t {};
    template <typename... Ts, template <typename> typename Predicate>
    struct any_of<type_list<Ts...>, Predicate> : bool_t<(Predicate<Ts>::value || ...)> {};
    template <typename List, template <typename> typename Predicate>
    constexpr bool any_of_v = any_of<List, Predicate>::value;

    // none_of
    template <typename List, template <typename> typename Predicate>
    constexpr bool none_of_v = !any_of_v<List, Predicate>;
    template <typename List, template <typename> typename Predicate>
    struct none_of : bool_t<none_of_v<List, Predicate>> {};

    // count_if
    template <typename List, template <typename> typename Predicate> struct count_if {};
    template <typename... Ts, template <typename> typename Predicate>
    struct count_if<type_list<Ts...>, Predicate> : size_t_t<(size_t(Predicate<Ts>::value) + ...)> {};
    template <typename List, template <typename> typename Predicate>
    constexpr size_t count_if_v = count_if<List, Predicate>::value;

    // bind_front
    template <template <typename...> typename Func, typename... Ts>
    struct bind_front
    {
        template <typename... Us>
        struct result { using type = Func<Ts..., Us...>; };
        template <typename... Us>
        using result_t = typename result<Us...>::type;
    };

    // count
    template <typename List, typename T>
    constexpr size_t count_v = count_if_v<List, bind_front<std::is_same, T>::template result_t>;
    template <typename List, typename T> struct count : size_t_t<count_v<List, T>> {};

    // contains
    template <typename List, typename T> constexpr bool contains_v = count_v<List, T> != 0;
    template <typename List, typename T> struct contains : bool_t<contains_v<List, T>> {};

    // transform
    template <typename List, template <typename> typename UnaryFunc> struct transform {};
    template <typename... Ts, template <typename> typename UnaryFunc>
    struct transform<type_list<Ts...>, UnaryFunc> { using type = type_list<UnaryFunc<Ts>...>; };
    template <typename List, template <typename> typename UnaryFunc>
    using transform_t = typename transform<List, UnaryFunc>::type;

}
