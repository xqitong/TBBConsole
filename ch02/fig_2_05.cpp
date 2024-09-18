#include "stdafx.h"
#include "fig_2_05.h"

 void ch02::fig_2_05::parallelQuicksort(QSVector::iterator b, QSVector::iterator e) {
    const int cutoff = 100;

    if (e - b < cutoff) {
        serialQuicksort(b, e);
    }
    else {
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
        tbb::parallel_invoke(
            [=]() { parallelQuicksort(b, i); },
            [=]() { parallelQuicksort(i + 1, e); }
        );
    }
}

 void ch02::fig_2_05::serialQuicksort(QSVector::iterator b, QSVector::iterator e) {
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

 QSVector ch02::fig_2_05::makeQSData(int N) {
    QSVector v;

    std::default_random_engine g;
    std::uniform_real_distribution<double> d(0.0, 1.0);
    v.reserve(N);
    for (int i = 0; i < N; ++i)
        v.push_back(DataItem{ i, d(g) });

    return v;
}

 bool ch02::fig_2_05::checkIsSorted(const QSVector& v) {
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

 void ch02::fig_2_05::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

 int ch02::fig_2_05::main() {
    const int N = 1000000;

    QSVector v = makeQSData(N);

    warmupTBB();
    double parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        parallelQuicksort(v.begin(), v.end());
        parallel_time = (tbb::tick_count::now() - t0).seconds();
        if (!checkIsSorted(v)) {
            std::cerr << "ERROR: tbb sorted list out-of-order" << std::endl;
        }
    }

    std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
    return 0;
}
