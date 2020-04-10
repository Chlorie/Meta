#include <typeinfo>
#include <iostream>

#include "meta/algorithm.h"
#include "meta/functional.h"

template <typename T>
void print_typename()
{
    std::cout << typeid(T).name() << '\n';
}

// NOLINTNEXTLINE
int main()
{
    constexpr size_t size = 100;
    using List = meta::to_type_list_t<meta::make_index_list<size>>;
    using List2 = meta::nth_types_t<List, meta::make_index_list<size>>;
    print_typename<List2>();
    return 0;
}
