#include "stdafx.h"
#include "sidebar_functor.h"
 using namespace ch01;
inline void ch01::sidebar_functor::PrintValues(int i, int j, int k, int l) {
    std::cout << "i == " << i << std::endl
        << "j == " << j << std::endl
        << "k == " << k << std::endl
        << "l == " << l << std::endl;
}

inline int ch01::sidebar_functor::main() {
    int i = 1, j = 10, k = 100, l = 1000;
    Functor f{ i,j };

    PrintValues(i, j, k, l);
    std::cout << "First call returned " << f(k, l) << std::endl;
    PrintValues(i, j, k, l);
    std::cout << "Second call returned " << f(k, l) << std::endl;
    PrintValues(i, j, k, l);
    return 0;
}


