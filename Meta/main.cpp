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
    using List = meta::to_type_list_t<meta::make_index_list<500>>;
    print_typename<meta::reverse_t<List>>();
    return 0;
}
