#include "stdafx.h"
#include "fig_3_06.h"

void ch03::fig_3_06::fig_3_6_run() {
    tbb::flow::graph g;

    int count = 0;
    tbb::flow::input_node<int> my_src{ g,
        [&count](tbb::flow_control& fc) -> int {
            const int limit = 3;
            int i = -1;
            if (count < limit) {
                i = count++;
                return i;
            }
            else {
                fc.stop();
                return i;
            }
        }
    };
    tbb::flow::function_node<int> my_node{ g,
        tbb::flow::unlimited,
        [](int i) {
            std::cout << std::setw(10) << std::setfill(' ') << std::this_thread::get_id() << std::setw(10) << " i = " << i << std::endl;
            //std::cout << i << std::endl;
        }
    };

    tbb::flow::make_edge(my_src, my_node);

    my_src.activate();

    g.wait_for_all();
}

void ch03::fig_3_06::loop_with_try_put() {
    const int limit = 30;
    tbb::flow::graph g;
    tbb::flow::function_node<int> my_node{ g, tbb::flow::unlimited,
        [](int i) {
            std::cout<<std::setw(10)<<std::setfill(' ') <<std::this_thread::get_id() << std::setw(10) << " i = " << i << std::endl;
        }
    };
    for (int count = 0; count < limit; ++count) {
        int value = count;
        my_node.try_put(value);
    }
    g.wait_for_all();
}

void ch03::fig_3_06::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

int ch03::fig_3_06::main() {
    warmupTBB();
    double try_put_time = 0.0, src_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        loop_with_try_put();
        try_put_time = (tbb::tick_count::now() - t0).seconds();
    }
    warmupTBB();
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_3_6_run();
        src_time = (tbb::tick_count::now() - t0).seconds();
    }

    std::cout << "try_put_time == " << try_put_time << " seconds" << std::endl;
    std::cout << "src_time == " << src_time << " seconds" << std::endl;
    return 0;
}
