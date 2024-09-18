#include "stdafx.h"
#include "fig_2_06.h"

 void ch02::fig_2_06::fig_2_06_run(int N, const std::vector<int>& a) {
    tbb::parallel_for(0, N, 1, [a](int i) {
        f(a[i]);
        });
}

 void ch02::fig_2_06::fig_2_06_std_run(int N, const std::vector<int>& a)
 { 
     std::for_each(std::execution::par, a.cbegin(), a.cend(),[](int i) {
         f(i);
         });
 }

 void ch02::fig_2_06::serialImpl(int N, const std::vector<int>& a) {
    for (int i = 0; i < N; ++i) {
        f(a[i]);
    }
}

 void ch02::fig_2_06::spinWaitForAtLeast(double sec) {
    if (sec == 0.0) return;
    tbb::tick_count t0 = tbb::tick_count::now();
    while ((tbb::tick_count::now() - t0).seconds() < sec);
}

 void ch02::fig_2_06::f(int v) {
    if (v % 2) {
        spinWaitForAtLeast(0.001);
    }
    else {
        spinWaitForAtLeast(0.002);
    }
}

 void ch02::fig_2_06::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

 int ch02::fig_2_06::main() {
    const int N = 1000;

    std::vector<int> v(N, 0);
    int i = 0;
    std::generate(v.begin(), v.end(), [&i]() { return i++; });

    double serial_time = 0.0, parallel_time = 0.0, std_parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        serialImpl(N, v);
        serial_time = (tbb::tick_count::now() - t0).seconds();
    }

    warmupTBB();
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_2_06_run(N, v);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
    }
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_2_06_std_run(N,v);
        std_parallel_time = (tbb::tick_count::now() - t0).seconds();
    }

    std::cout << "serial_time == " << serial_time << " seconds" << std::endl
        << "parallel_time == " << parallel_time << " seconds" << std::endl
        << "speedup == " << serial_time / parallel_time << std::endl
        << "std_parallel_time == " << std_parallel_time << " seconds" << std::endl 
        << "std_speedup == " << serial_time / std_parallel_time << std::endl;

    return 0;
}
