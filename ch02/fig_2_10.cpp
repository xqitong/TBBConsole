#include "stdafx.h"
#include "fig_2_10.h"

 int ch02::fig_2_10::fig_2_10_run(const std::vector<int>& a) {
    int max_value = tbb::parallel_reduce(
        /* the range = */ tbb::blocked_range<int>(0, a.size()),
        /* identity = */ std::numeric_limits<int>::min(),
        /* func = */
        [&](const tbb::blocked_range<int>& r, int init) -> int {
            for (int i = r.begin(); i != r.end(); ++i) {
                init = std::max(init, a[i]);
            }
            return init;
        },
        /* reduction = */
        [](int x, int y) -> int {
            return std::max(x, y);
        }
    );
    return max_value;
}

 int ch02::fig_2_10::serialImpl(const std::vector<int>& a) {
    int max_value = std::numeric_limits<int>::min();
    for (int i = 0; i < a.size(); ++i) {
        max_value = std::max(max_value, a[i]);
    }
    return max_value;
}

 void ch02::fig_2_10::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

 int ch02::fig_2_10::main() {
    const int N = INT_MAX;
    std::vector<int> a(N, 0);
    for (int i = 0; i < N; ++i) {
        a[i] = std::abs(i - N / 2);
    }

    double serial_time = 0.0, parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        int max_value = serialImpl(a);
        serial_time = (tbb::tick_count::now() - t0).seconds();
        if (max_value != N / 2) {
            std::cerr << "ERROR: serial max_value is wrong!" << std::endl;
        }
    }

    warmupTBB();
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        int max_value = fig_2_10_run(a);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
        if (max_value != N / 2) {
            std::cerr << "ERROR: parallel max_value is wrong!" << std::endl;
        }
    }

    std::cout << "serial_time == " << serial_time << " seconds" << std::endl
        << "parallel_time == " << parallel_time << " seconds" << std::endl
        << "speedup == " << serial_time / parallel_time << std::endl;

    return 0;
}
