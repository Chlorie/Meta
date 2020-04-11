#pragma once

#include "common.h"

namespace meta
{
#define META_DEFINE_BINARY_OP(name, symbol) \
    template <typename T, typename U> using name = integral_t<(T::value symbol U::value)>

    // Arithmetic
    META_DEFINE_BINARY_OP(plus, +);
    META_DEFINE_BINARY_OP(minus, -);
    META_DEFINE_BINARY_OP(multiplies, *);
    META_DEFINE_BINARY_OP(divides, / );
    META_DEFINE_BINARY_OP(modulus, %);
    template <typename T> using negate = integral_t<-T::value>;

    // Comparison
    META_DEFINE_BINARY_OP(equal_to, == );
    META_DEFINE_BINARY_OP(not_equal_to, != );
    META_DEFINE_BINARY_OP(greater, > );
    META_DEFINE_BINARY_OP(less, < );
    META_DEFINE_BINARY_OP(greater_equal, >= );
    META_DEFINE_BINARY_OP(less_equal, <= );

    // Logical
    META_DEFINE_BINARY_OP(logical_and, &&);
    META_DEFINE_BINARY_OP(logical_or, || );
    template <typename T> using logical_not = integral_t<!T::value>;

    // Bitwise
    META_DEFINE_BINARY_OP(bit_and, &);
    META_DEFINE_BINARY_OP(bit_or, | );
    META_DEFINE_BINARY_OP(bit_xor, ^);
    template <typename T> using bit_not = integral_t<~T::value>;

#undef META_DEFINE_BINARY_OP

    // Help functions
    template <typename T, typename> using first = T;

    // Binary fold in 256 chunks, for the compiler to not overflow its stack
    namespace fold
    {
#define META_DEFINE_MACROS
#include "macros.h"

#define META_DEFINE_FOLD_OP(Type, name, symbol) \
        template <Type... Values> struct name##_direct : integral_t<(Values symbol ...)> {}; \
        template <Type... Values> struct name : name##_direct<Values...> {}; \
        template <META_EXPAND_256(Type V), Type... Rest> \
        struct name<META_EXPAND_256(V), Rest...> : \
            integral_t<(name##_direct<META_EXPAND_256(V)>::value symbol name<Rest...>::value)> {}; \
        template <Type... Values> constexpr Type name##_v = name<Values...>::value

        META_DEFINE_FOLD_OP(auto, plus, +);
        META_DEFINE_FOLD_OP(bool, logical_and, &&);
        META_DEFINE_FOLD_OP(bool, logical_or, ||);

#undef META_DEFINE_FOLD_OP

#undef META_DEFINE_MACROS
#include "macros.h"
    }
}
