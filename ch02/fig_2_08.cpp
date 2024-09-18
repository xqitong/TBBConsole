#include "stdafx.h"
#include "fig_2_08.h"

 void ch02::fig_2_08::fig_2_08_run(int M, double* a, double* b, double* c) {
    tbb::parallel_for(0, M, [=](int i) {
        for (int j = 0; j < M; ++j) {
            int c_index = i * M + j;
            for (int k = 0; k < M; ++k) {
                c[c_index] += a[i * M + k] * b[k * M + j];
            }
        }
        });
}

 void ch02::fig_2_08::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

 int ch02::fig_2_08::main() {
    const int M = 1024;
    const int MxM = M * M;
    double* a = new double[MxM];
    double* b = new double[MxM];
    double* c = new double[MxM];
    double* c_expected = new double[MxM];

    // init arrays
    std::fill(a, a + MxM, 1.0);
    std::fill(b, b + MxM, 1.0);
    std::fill(c, c + MxM, 0.0);
    std::fill(c_expected, c_expected + MxM, M);


    warmupTBB();
    double parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_2_08_run(M, a, b, c);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
        if (!std::equal(c, c + MxM, c_expected)) {
            std::cerr << "ERROR: c array does not match expected values" << std::endl;
        }
    }

    std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] c_expected;
    return 0;
}
