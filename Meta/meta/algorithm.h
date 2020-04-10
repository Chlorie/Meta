#pragma once

#include "list.h"
#include "functional.h"

namespace meta
{
    // apply
    template <typename List, template <typename...> typename Templ> struct apply {};
    template <typename... Ts, template <typename...> typename Templ>
    struct apply<type_list<Ts...>, Templ> { using type = Templ<Ts...>; };
    template <typename List, template <typename...> typename Templ>
    using apply_t = typename apply<List, Templ>::type;

    // size_of
    template <typename List> constexpr size_t size_of_v = List::size;
    template <typename List> struct size_of : size_t_t<size_of_v<List>> {};

    // is_empty
    template <typename List> constexpr bool is_empty_v = List::size == 0;
    template <typename List> struct is_empty : bool_t<is_empty_v<List>> {};

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

    // reverse
    namespace detail
    {
        template <size_t N> // Reversed index sequence (from N-1 to 0)
        using rev_ilist = to_index_list_t<transform_t<
            to_type_list_t<make_index_list<N>>, bind_front<minus, size_t_t<N - 1>>::template result_t>>;
    }
    template <typename List> struct reverse : nth_types<List, detail::rev_ilist<List::size>> {};
    template <typename List> using reverse_t = typename reverse<List>::type;
}
