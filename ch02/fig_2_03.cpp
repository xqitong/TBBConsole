#include "stdafx.h"
#include "fig_2_03.h"
#define NOMINMAX

 void ch02::fig_2_03::fig_2_3_run(QSVector& v1, QSVector& v2) {
    tbb::parallel_invoke(
        [&v1]() { serialQuicksort(v1.begin(), v1.end()); },
        [&v2]() { serialQuicksort(v2.begin(), v2.end()); }
    );
}

 QSVector ch02::fig_2_03::makeQSData(int N) {
    QSVector v;

    std::default_random_engine g;
    std::uniform_real_distribution<double> d(0.0, 1.0);
    v.reserve(N);
    for (int i = 0; i < N; ++i)
        v.push_back(DataItem{ i, d(g) });

    return v;
}

 bool ch02::fig_2_03::checkIsSorted(const QSVector& v) {
    double max_value = std::numeric_limits<double>::min();
    for (auto e : v) {
        if (e.value < max_value) {
            std::cerr << "Sort FAILED" << std::endl;
            return false;
        }
        max_value = e.value;
    }
    return true;
}

 void ch02::fig_2_03::serialQuicksort(QSVector::iterator b, QSVector::iterator e) {
    if (b >= e) return;

    // do shuffle
    double pivot_value = b->value;
    QSVector::iterator i = b, j = e - 1;
    while (i != j) {
        while (i != j && pivot_value < j->value) --j;
        while (i != j && i->value <= pivot_value) ++i;
        std::iter_swap(i, j);
    }
    std::iter_swap(b, i);

    // recursive call
    serialQuicksort(b, i);
    serialQuicksort(i + 1, e);
}

 void ch02::fig_2_03::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

 int ch02::fig_2_03::main() {
    const int N = 1000000;

    QSVector serial_v1 = makeQSData(N);
    QSVector serial_v2 = makeQSData(N);
    QSVector tbb_v1 = makeQSData(N);
    QSVector tbb_v2 = makeQSData(N);

    double serial_time = 0.0, parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        serialQuicksort(serial_v1.begin(), serial_v1.end());
        serialQuicksort(serial_v2.begin(), serial_v2.end());
        serial_time = (tbb::tick_count::now() - t0).seconds();
        if (!checkIsSorted(serial_v1) || !checkIsSorted(serial_v2)) {
            std::cerr << "ERROR: serial sorted list out-of-order" << std::endl;
        }
    }

    warmupTBB();
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_2_3_run(tbb_v1, tbb_v2);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
        if (!checkIsSorted(tbb_v1) || !checkIsSorted(tbb_v2)) {
            std::cerr << "ERROR: tbb sorted list out-of-order" << std::endl;
        }
    }

    std::cout << "serial_time == " << serial_time << " seconds" << std::endl
        << "parallel_time == " << parallel_time << " seconds" << std::endl
        << "speedup == " << serial_time / parallel_time << std::endl;

    return 0;
}
