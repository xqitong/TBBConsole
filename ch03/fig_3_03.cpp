#include "stdafx.h"
#include "fig_3_03.h"

void ch03::fig_3_03::fig_3_3_run() {
    // step 1: construct the graph
    tbb::flow::graph g;

    // step 2: make the nodes
    tbb::flow::function_node<int, std::string> my_first_node{ g,
        tbb::flow::unlimited,
        [](const int& in) -> std::string {
            std::cout << "first node received: " << in << std::endl;
            return std::to_string(in);
        }
    };

    tbb::flow::function_node<std::string> my_second_node{ g,
            tbb::flow::unlimited,
            [](const std::string& in) {
                std::cout << "second node received: " << in << std::endl;
            }
    };

    // step 3: add edges
    tbb::flow::make_edge(my_first_node, my_second_node);

    // step 4: send messages
    my_first_node.try_put(10);

    // step 5: wait for graph to complete
    g.wait_for_all();
}

void ch03::fig_3_03::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

int ch03::fig_3_03::main() {
    warmupTBB();
    double parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_3_3_run();
        parallel_time = (tbb::tick_count::now() - t0).seconds();
    }

    std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
    return 0;
}
