#include "stdafx.h"
#include "fig_2_04.h"
#define NOMINMAX     

 void ch02::fig_2_04::serialQuicksort(QSVector::iterator b, QSVector::iterator e) {
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

 QSVector ch02::fig_2_04::makeQSData(int N) {
    QSVector v;

    std::default_random_engine g;
    std::uniform_real_distribution<double> d(0.0, 1.0);

    for (int i = 0; i < N; ++i)
        v.push_back(DataItem{ i, d(g) });

    return v;
}

 bool ch02::fig_2_04::checkIsSorted(const QSVector& v) {
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

 int ch02::fig_2_04::main() {
    const int N = 1000000;

    QSVector v = makeQSData(N);
    double serial_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        serialQuicksort(v.begin(), v.end());
        serial_time = (tbb::tick_count::now() - t0).seconds();
        if (!checkIsSorted(v)) {
            std::cerr << "ERROR: serial sorted list out-of-order" << std::endl;
        }
    }
    std::cout << "serial_time == " << serial_time << " seconds" << std::endl;
    return 0;
}
