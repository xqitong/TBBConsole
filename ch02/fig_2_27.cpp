#include "stdafx.h"
#include "fig_2_27.h"
tbb::concurrent_queue<CaseStringPtr> ch02::fig_2_27::caseFreeList{};
int ch02::fig_2_27::numCaseInputs = 0;
void ch02::fig_2_27::fig_2_27_run(int num_tokens, std::ofstream& caseBeforeFile, std::ofstream& caseAfterFile) {
    tbb::parallel_pipeline(
        /* tokens */ num_tokens,
        /* the get filter */
        tbb::make_filter<void, CaseStringPtr>(
            /* filter node */ tbb::filter_mode::serial_in_order,
            /* filter body */
            [&](tbb::flow_control& fc) -> CaseStringPtr {
                CaseStringPtr s_ptr = getCaseString(caseBeforeFile);
                if (!s_ptr)
                    fc.stop();
                return s_ptr;
            }) & // concatenation operation
        /* make the change case filter */
                tbb::make_filter<CaseStringPtr, CaseStringPtr>(
                    /* filter node */ tbb::filter_mode::parallel,
                    /* filter body */
                    [](CaseStringPtr s_ptr) -> CaseStringPtr {
                        std::transform(s_ptr->begin(), s_ptr->end(), s_ptr->begin(),
                            [](char c) -> char {
                                if (std::islower(c))
                                    return std::toupper(c);
                                else if (std::isupper(c))
                                    return std::tolower(c);
                                else
                                    return c;
                            });
                        return s_ptr;
                    }) & // concatenation operation
                /* make the write filter */
                        tbb::make_filter<CaseStringPtr, void>(
                            /* filter node */ tbb::filter_mode::serial_in_order,
                            /* filter body */
                            [&](CaseStringPtr s_ptr) -> void {
                                writeCaseString(caseAfterFile, s_ptr);
                            })
                        );
}

void ch02::fig_2_27::initCaseChange(int num_strings, int string_len, int free_list_size) {
    numCaseInputs = num_strings;
    caseFreeList.clear();
    for (int i = 0; i < free_list_size; ++i) {
        caseFreeList.push(std::make_shared<std::string>(string_len, ' '));
    }
}

CaseStringPtr ch02::fig_2_27::getCaseString(std::ofstream& f) {
    std::shared_ptr<std::string> s;
    if (numCaseInputs > 0) {
        if (!caseFreeList.try_pop(s) || !s) {
            std::cerr << "Error: Ran out of elements in free list!" << std::endl;
            return CaseStringPtr{};
        }
        int ascii_range = 'z' - 'A' + 2;
        for (int i = 0; i < s->size(); ++i) {
            int offset = i % ascii_range;
            if (offset)
                (*s)[i] = 'A' + offset - 1;
            else
                (*s)[i] = '\n';
        }
        if (f.good()) {
            f << *s;
        }
        --numCaseInputs;
    }
    return s;
}

void ch02::fig_2_27::writeCaseString(std::ofstream& f, CaseStringPtr s) {
    if (f.good()) {
        f << *s;
    }
    caseFreeList.push(s);
}

void ch02::fig_2_27::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

int ch02::fig_2_27::main() {
    int num_tokens = tbb::this_task_arena::max_concurrency();
    int num_strings = 100;
    int string_len = 100000;
    int free_list_size = num_tokens;

    std::ofstream caseBeforeFile("fig_2_27_before.txt");
    std::ofstream caseAfterFile("fig_2_27_after.txt");
    initCaseChange(num_strings, string_len, free_list_size);

    warmupTBB();
    double parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_2_27_run(num_tokens, caseBeforeFile, caseAfterFile);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
    }
    std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
    return 0;
}
