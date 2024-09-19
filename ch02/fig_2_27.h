/*
Copyright (C) 2019 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*/

#pragma once
namespace ch02 {
    struct fig_2_27 {
        static tbb::concurrent_queue<CaseStringPtr> caseFreeList;
        static int numCaseInputs;
        static void fig_2_27_run(int num_tokens, std::ofstream& caseBeforeFile, std::ofstream& caseAfterFile);
        static void initCaseChange(int num_strings, int string_len, int free_list_size);
        static CaseStringPtr getCaseString(std::ofstream& f);
        static void writeCaseString(std::ofstream& f, CaseStringPtr s);
        static void warmupTBB();
        static int main();
    };
}
