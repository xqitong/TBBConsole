#pragma once
#include <vector>
#include <filesystem>
#include <windows.h>  
namespace ch02 {
    class Helper {
    public:
        static std::string GetExePath() {
            char path[MAX_PATH];
            if (GetModuleFileNameA(NULL, path, MAX_PATH) != 0) {
                std::cout << "Executable path: " << path << std::endl;
            }
            else {
                std::cerr << "Failed to get the executable path.\n";
            }
            return path;
        }
        static std::string GetFolder(const std::string& file_path) {
            auto index = file_path.find_last_of('\\');
            // 将std::string转换为fs::path  
            std::filesystem::path path(file_path);

            // 获取父路径，即文件夹路径  
            std::filesystem::path folder_path = path.parent_path();

            // 将fs::path转换回std::string  
            return folder_path.string();
        }
    };
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