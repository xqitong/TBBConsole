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
    class PNGImage {
    public:
        uint64_t frameNumber = -1;
        unsigned int width = 0, height = 0;
        std::shared_ptr<std::vector<unsigned char>> buffer;
        static const int numChannels = 4;
        static const int redOffset = 0;
        static const int greenOffset = 1;
        static const int blueOffset = 2;

        PNGImage() {}
        PNGImage(uint64_t frame_number, const std::string& file_name);
        PNGImage(const PNGImage& p);
        virtual ~PNGImage() {}
        void write() const;
    };
    struct fig_2_28 {
        static int stereo3DFrameCounter;
        static int stero3DNumImages;
        static void fig_2_28_run();
        static void initStereo3D(int num_images);
        static int getNextFrameNumber();
        static PNGImage getLeftImage(uint64_t frameNumber);
        static PNGImage getRightImage(uint64_t frameNumber);
        static void increasePNGChannel(PNGImage& image, int channel_offset, int increase);
        static void mergePNGImages(PNGImage& right, const PNGImage& left);
        static int main();
    };
}
