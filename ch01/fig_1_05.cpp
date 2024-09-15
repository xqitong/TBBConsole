#include "stdafx.h"
#include "fig_1_05.h"

 int ch01::fig_1_05::main() {
    std::vector<std::string> v = { " Hello ", " Parallel STL! " };
    std::for_each(std::execution::par, v.begin(), v.end(),
        [](std::string& s) { std::cout << s << std::endl; });
    return 0;
}
