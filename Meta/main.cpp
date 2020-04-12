#include <typeinfo>
#include <iostream>

#include "meta/algorithm.h"

template <typename T>
void print_typename()
{
    std::cout << typeid(T).name() << '\n';
}

// NOLINTNEXTLINE
int main()
{
    constexpr size_t size = 5000;
    using T = meta::repeat_t<void, size + 1>;
    using U = meta::repeat_t<void, size>;
    constexpr size_t res = meta::mismatch_v<T, U>;
    std::cout << res << '\n';
    return 0;
}
