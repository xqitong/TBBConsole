#include "stdafx.h"
#include "fig_2_12.h"

 double ch02::fig_2_12::fig_2_12_run(int num_intervals) {
    double dx = 1.0 / num_intervals;
    double sum = tbb::parallel_reduce(
        /* range = */ tbb::blocked_range<int>(0, num_intervals),
        /* idenity = */ 0.0,
        /* func */
        [=](const tbb::blocked_range<int>& r, double init) -> double {
            for (int i = r.begin(); i != r.end(); ++i) {
                double x = (i + 0.5) * dx;
                double h = std::sqrt(1 - x * x);
                init += h * dx;
            }
            return init;
        },
        /* reduction */
        [](double x, double y) -> double {
            return x + y;
        }
    );
    double pi = 4 * sum;
    return pi;
}

 void ch02::fig_2_12::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

 int ch02::fig_2_12::main() {
    const int num_intervals = std::numeric_limits<int>::max();
    double parallel_time = 0.0;
    warmupTBB();
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        double pi = fig_2_12_run(num_intervals);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
        std::cout << "parallel pi == " << std::setprecision(16) << pi << std::endl;
    }

    std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
    return 0;
}
