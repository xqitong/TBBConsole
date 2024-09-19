#pragma once
#include <vector>
namespace ch02 {

    struct DataItem {
        int id;
        double value;
        DataItem(int i, double v) : id{ i }, value{ v } {}
    };

    using QSVector = std::vector<DataItem>; 
/////////////////////////////////////////////////////////
    using PrimesValue = std::pair<int, bool>;
    using PrimesList = std::list<PrimesValue>;
    using PrimesMap = std::map<int, bool>;
    using IntVector = std::vector<int>;
  
    struct PrimesTreeElement {
        using Ptr = std::shared_ptr<PrimesTreeElement>;

        PrimesValue v;
        Ptr left;
        Ptr right;
        PrimesTreeElement(const PrimesValue& _v) : left{}, right{} {
            v.first = _v.first;
            v.second = _v.second;
        }
    };

    using CaseStringPtr = std::shared_ptr<std::string>;

    class Timer {
        std::chrono::time_point<std::chrono::system_clock> startTime;
        bool reportAtDestruction;

    public:
        Timer(bool report = true) : startTime{ std::chrono::system_clock::now() }, reportAtDestruction{ report } {}

        virtual ~Timer() {
            if (reportAtDestruction)
                std::cout << "Timer : " << elapsed_seconds() << " seconds" << std::endl;
        }

        double elapsed_seconds() const {
            std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - startTime;
            return elapsed.count();
        }

        int64_t elapsed_nanoseconds() const {
            std::chrono::duration<int64_t, std::nano> elapsed = std::chrono::system_clock::now() - startTime;
            return elapsed.count();
        }

        void reset() {
            startTime = std::chrono::system_clock::now();
        }
    };

    struct common {

        static void spinWaitForAtLeast(double sec) {
            if (sec == 0.0) return;
            tbb::tick_count t0 = tbb::tick_count::now();
            while ((tbb::tick_count::now() - t0).seconds() < sec);
        }

        static void warmupTBB(double per_body_time = 0.01, int P = tbb::this_task_arena::max_concurrency()) {
            tbb::parallel_for(0, P, [per_body_time](int) {
                spinWaitForAtLeast(per_body_time);
                });
        }
    };
} // namespace ch02