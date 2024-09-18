#include "stdafx.h"
#include "fig_2_16.h"
std::vector< std::string> ch02::fig_2_16::LOSInputNames = { "flat", "decreasing", "wave", "plateau" };

 void ch02::fig_2_16::fig_2_16_run(const std::vector<double>& altitude, std::vector<bool>& is_visible, double dx) {
    const int N = altitude.size();
    double max_angle = std::atan2(dx, altitude[0] - altitude[1]);

    double final_max_angle = tbb::parallel_scan(
        /*  range = */ tbb::blocked_range<int>(1, N),
        /* identity */ 0.0,
        /* scan body */
        [&altitude, &is_visible, dx](const tbb::blocked_range<int>& r,
            double max_angle,
            bool is_final_scan) -> double {
                for (int i = r.begin(); i != r.end(); ++i) {
                    double my_angle = atan2(i * dx, altitude[0] - altitude[i]);
                    if (my_angle >= max_angle)
                        max_angle = my_angle;
                    if (is_final_scan && my_angle < max_angle)
                        is_visible[i] = false;
                }
                return max_angle;
        },
        [](double a, double b) -> double {
            return std::max(a, b);
        }
    );
}

std::vector<double> ch02::fig_2_16::makeAltitudeVector(int N, LOSInputType t) {
    std::vector<double> v(N);

    const int saw_period = N / 10;
    double previous_value = N;
    double delta = -1;
    for (int i = 0; i < N; ++i) {
        switch (t) {
        case ALT_FLAT:
            if (i == 0)
                v[i] = N;
            else
                v[i] = N + 10;
            break;
        case ALT_DECREASING:
            if (i == 0) {
                v[i] = N;
                previous_value = -1;
            }
            else {
                v[i] = (++previous_value);
            }
            break;
        case ALT_WAVE:
            if (i == 0) {
                v[i] = previous_value = N;
                delta = -1;
            }
            else {
                v[i] = (previous_value += delta);
                if (i % saw_period == 0) delta *= -1;
            }
            break;
        case ALT_PLATEAU:
            if (i == 0)
                v[i] = N;
            else if (i < N / 10)
                v[i] = N / 2;
            else
                v[i] = N / 4;
            break;
        }
    }
    return v;
}

void ch02::fig_2_16::printVisibility(bool b, int f, int l) {
    std::cerr << f << " - " << l << " : ";
    if (b) {
        std::cerr << "visible" << std::endl;
    }
    else {
        std::cerr << "not visible" << std::endl;
    }
}

bool ch02::fig_2_16::checkVisibility(LOSInputType t, std::vector<bool>& is_visible) {
    const int N = is_visible.size();
    const int saw_period = N / 10;
    int i = 0;

    switch (t) {
    case ALT_FLAT:
        if (!is_visible[0] || !is_visible[1]) {
            return false;
        }
        for (i = 2; i < N; ++i) {
            if (is_visible[i] == true)
                return false;
        }
        return true;
        break;
    case ALT_DECREASING:
        for (i = 0; i < N; ++i) {
            if (is_visible[i] == false)
                return false;
        }
        return true;
        break;
    case ALT_WAVE:
        for (i = 0; i < 2 * saw_period; ++i) {
            if (is_visible[i] == false)
                return false;
        }
        for (i = 2 * saw_period; i < N; i += 2 * saw_period) {
            if (is_visible[i] == false)
                return false;
        }
        return true;
        break;
    case ALT_PLATEAU:
        // first part is visible
        i = 0;
        for (; i < saw_period; ++i) {
            if (is_visible[i] == false)
                return false;
        }
        // second part is not visible
        while (is_visible[i] == false && i < N) {
            ++i;
        }
        if (i <= saw_period || i == N)
            return false;
        // third part is visible
        for (; i < N; ++i) {
            if (is_visible[i] == false)
                return false;
        }
        return true;
        break;
    default:
        return false;
    }
}

void ch02::fig_2_16::dumpLOS(std::vector<bool>& is_visible) {
    bool can_see_it = true;
    const int N = (int)is_visible.size();
    int first_index = 0;
    for (int i = 0; i < N; ++i) {
        bool b = is_visible[i];
        if (can_see_it != b) {
            printVisibility(can_see_it, first_index, i - 1);
            can_see_it = b;
            first_index = i;
        }
    }
    printVisibility(is_visible[N - 1], first_index, N - 1);
}

void ch02::fig_2_16::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

int ch02::fig_2_16::main() {
    const int N = 100000000;
    const double dx = 1;

    auto input_types = { ALT_FLAT, ALT_DECREASING, ALT_WAVE, ALT_PLATEAU };
    std::vector<bool> is_visible(N, true);

    for (auto i : input_types) {
        std::vector<double> altitude = makeAltitudeVector(N, i);
        std::fill(is_visible.begin(), is_visible.end(), true);

        double parallel_time = 0.0;
        {
            warmupTBB();
            tbb::tick_count t0 = tbb::tick_count::now();
            fig_2_16_run(altitude, is_visible, 1.0);
            parallel_time = (tbb::tick_count::now() - t0).seconds();
            dumpLOS(is_visible);
            if (checkVisibility(i, is_visible) != true) {
                std::cerr << "ERROR: visibility is not as expected!" << std::endl;
            }
        }
        std::cout << "parallel_time for " << LOSInputNames[i] << " == " << parallel_time << " seconds" << std::endl;
    }
    return 0;
}
