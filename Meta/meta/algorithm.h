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
    struct all_of<type_list<Ts...>, Predicate> : fold::logical_and<Predicate<Ts>::value...> {};
    template <typename List, template <typename> typename Predicate>
    constexpr bool all_of_v = all_of<List, Predicate>::value;

    // any_of
    template <typename List, template <typename> typename Predicate> struct any_of : true_t {};
    template <typename... Ts, template <typename> typename Predicate>
    struct any_of<type_list<Ts...>, Predicate> : fold::logical_or<Predicate<Ts>::value...> {};
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
    struct count_if<type_list<Ts...>, Predicate> : fold::plus<size_t(Predicate<Ts>::value)...> {};
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

    // repeat
    template <typename T, size_t N>
    using repeat_t = transform_t<make_index_type_list<N>, bind_front<first, T>::template result_t>;
    template <typename T, size_t N> struct repeat { using type = repeat_t<T, N>; };

    // mismatch
    namespace detail
    {
        struct min_wrapper { size_t value = 0; };
        constexpr min_wrapper operator+(const min_wrapper lhs, const min_wrapper rhs)
        {
            return { lhs.value < rhs.value ? lhs.value : rhs.value };
        }
        template <typename... Ts, typename... Us, size_t... Idx>
        constexpr size_t mismatch_impl(
            type_list<Ts...>, type_list<Us...>, std::index_sequence<Idx...>)
        {
            // Write the fold operand "std::is_same_v<Ts, Us> ? size_t(-1) : Idx" as oper,
            // the returned value is basically (... + min_wrapper{ oper }).value,
            // which is simply (... min oper) if we treat the min function like an operator
            return fold::wrapped_plus_v<min_wrapper,
                (std::is_same_v<Ts, Us> ? size_t(-1) : Idx)...>.value;
        }
    }
    template <typename First, typename Second> struct mismatch {};
    template <typename... Ts, typename... Us>
    struct mismatch<type_list<Ts...>, type_list<Us...>>
    {
    private:
        template <bool V> struct unmatchable {};
        static constexpr size_t tsize = sizeof...(Ts), usize = sizeof...(Us);
        static constexpr size_t size = (tsize > usize ? tsize : usize) + 1;
        using tlist = concat_t<type_list<Ts...>, repeat_t<unmatchable<false>, size - tsize>>;
        using ulist = concat_t<type_list<Us...>, repeat_t<unmatchable<true>, size - usize>>;
    public:
        static constexpr size_t value = detail::mismatch_impl(
            tlist{}, ulist{}, make_index_list<size>{});
    };
    template <typename First, typename Second> constexpr size_t mismatch_v = mismatch<First, Second>::value;

    // subseq
    namespace detail
    {
        template <size_t Begin, size_t Size> // Index range
        using index_range = to_index_list_t<transform_t<
            make_index_type_list<Size>, bind_front<plus, size_t_t<Begin>>::template result_t>>;
    }
    template <typename List, size_t Begin, size_t Size>
    using subseq_t = at_list_t<List, detail::index_range<Begin, Size>>;
    template <typename List, size_t Begin, size_t Size>
    struct subseq { using type = subseq_t<List, Begin, Size>; };

    // reverse
    namespace detail
    {
        template <size_t N> // Reversed index sequence (from N-1 to 0)
        using rev_ilist = to_index_list_t<transform_t<
            make_index_type_list<N>, bind_front<minus, size_t_t<N - 1>>::template result_t>>;
    }
    template <typename List> using reverse_t = at_list_t<List, detail::rev_ilist<List::size>>;
    template <typename List> struct reverse { using type = reverse_t<List>; };
}
