#include "stdafx.h"
#include "fig_2_14.h"

 int ch02::fig_2_14::fig_2_14_run(const std::vector<int>& v, std::vector<int>& rsum) {
    int N = v.size();
    rsum[0] = v[0];
    int final_sum = tbb::parallel_scan(
        /* range = */ tbb::blocked_range<int>(1, N),
        /* identity = */ (int)0,
        /* scan body */
        [&v, &rsum](const tbb::blocked_range<int>& r,
            int sum, bool is_final_scan) -> int {
                for (int i = r.begin(); i < r.end(); ++i) {
                    sum += v[i];
                    if (is_final_scan)
                        rsum[i] = sum;
                }
                return sum;
        },
        /* combine body */
        [](int x, int y) {
            return x + y;
        }
    );
    return final_sum;
}

 int ch02::fig_2_14::serialImpl(const std::vector<int>& v, std::vector<int>& rsum) {
    int N = v.size();
    rsum[0] = v[0];
    for (int i = 1; i < N; ++i) {
        rsum[i] = rsum[i - 1] + v[i];
    }
    int final_sum = rsum[N - 1];
    return final_sum;
}

 void ch02::fig_2_14::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

 int ch02::fig_2_14::main() {
    const int N = 1e8;
    std::vector<int> v(N, 0);
    std::vector<int> serial_rsum(N, 0);
    std::vector<int> parallel_rsum(N, 0);
    for (int i = 0; i < N; ++i) {
        v[i] = i;
    }

    double serial_time = 0.0, parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        int final_sum = serialImpl(v, serial_rsum);
        serial_time = (tbb::tick_count::now() - t0).seconds();
        if (final_sum != N * (N - 1) / 2) {
            std::cerr << "ERROR: serial final_sum is wrong! "
                << final_sum << " != " << N * (N - 1) / 2
                << std::endl;
        }
    }

    warmupTBB();
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        int final_sum = fig_2_14_run(v, parallel_rsum);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
        if (final_sum != N * (N - 1) / 2) {
            std::cerr << "ERROR: parallel final_sum is wrong! "
                << final_sum << " != " << N * (N - 1) / 2
                << std::endl;
        }
    }

    if (serial_rsum != parallel_rsum) {
        std::cerr << "ERROR: rsum vectors do not match!" << std::endl;
    }

    std::cout << "serial_time == " << serial_time << " seconds" << std::endl
        << "parallel_time == " << parallel_time << " seconds" << std::endl
        << "speedup == " << serial_time / parallel_time << std::endl;

    return 0;
}
