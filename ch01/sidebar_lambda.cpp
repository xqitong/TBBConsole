#include "stdafx.h"
#include "sidebar_lambda.h"

inline void ch01::sidebar_lambda::printValues(int i, int j, int k, int l) {
    std::cout << "i == " << i << std::endl
        << "j == " << j << std::endl
        << "k == " << k << std::endl
        << "l == " << l << std::endl;
}

inline int ch01::sidebar_lambda::main() {
    int i = 1, j = 10, k = 100, l = 1000;
    auto lambdaExpression = [i, &j](int k0, int& l0) -> int {
        j = 2 * j;
        k0 = 2 * k0;
        l0 = 2 * l0;
        return i + j + k0 + l0;
    };

    printValues(i, j, k, l);
    std::cout << "First call returned " << lambdaExpression(k, l) << std::endl;
    printValues(i, j, k, l);
    std::cout << "Second call returned " << lambdaExpression(k, l) << std::endl;
    printValues(i, j, k, l);
    return 0;
}


