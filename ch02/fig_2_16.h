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
    struct fig_2_16 {
        enum LOSInputType { ALT_FLAT = 0, ALT_DECREASING, ALT_WAVE, ALT_PLATEAU };
        static std::vector< std::string> LOSInputNames;
        static void fig_2_16_run(const std::vector<double>& altitude, std::vector<bool>& is_visible, double dx);

        static std::vector<double> makeAltitudeVector(int N, LOSInputType t);

        static void printVisibility(bool b, int f, int l);

        static bool checkVisibility(LOSInputType t, std::vector<bool>& is_visible);

        static void dumpLOS(std::vector<bool>& is_visible);

        static void warmupTBB();

        static int main();
    };
}
