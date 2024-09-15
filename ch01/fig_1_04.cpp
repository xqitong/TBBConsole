#include "stdafx.h"
#include "fig_1_04.h"

 int ch01::fig_1_04::main() {
    tbb::parallel_invoke(
        []() { std::cout << " Hello " << std::endl; },
        []() { std::cout << " TBB! " << std::endl; }
    );
    return 0;
}
