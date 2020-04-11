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
    using Rep = meta::repeat_t<void, 10000>;
    constexpr bool v = meta::all_of_v<Rep, std::is_void>;
    constexpr size_t c = meta::count_v<Rep, void>;
    print_typename<Rep>();
    std::cout << std::boolalpha << v << ' ' << c << '\n';
    return 0;
}
