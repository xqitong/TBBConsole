#include "stdafx.h"
#include "fig_2_11.h"

 double ch02::fig_2_11::fig_2_11_run(int num_intervals) {
    double dx = 1.0 / num_intervals;
    double sum = 0.0;
    for (int i = 0; i < num_intervals; ++i) {
        double x = (i + 0.5) * dx;
        double h = std::sqrt(1 - x * x);
        sum += h * dx;
    }
    double pi = 4 * sum;
    return pi;
}

 int ch02::fig_2_11::main() {
    const int num_intervals = std::numeric_limits<int>::max();
    double serial_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        double pi = fig_2_11_run(num_intervals);
        serial_time = (tbb::tick_count::now() - t0).seconds();
        std::cout << "serial pi == "<<std::setprecision(16) << pi << std::endl;
    }

    std::cout << "serial_time == " << serial_time << " seconds" << std::endl;
    return 0;
}
