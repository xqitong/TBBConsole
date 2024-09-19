#include "stdafx.h"
#include "fig_2_28.h"
#include "lodepng.h"
PNGImage::PNGImage(uint64_t frame_number, const std::string& file_name) :
    frameNumber{ frame_number }, buffer{ std::make_shared< std::vector<unsigned char> >() } {
    if (lodepng::decode(*buffer, width, height, file_name)) {
        std::cerr << "Error: could not read PNG file!" << std::endl;
        width = height = 0;
    }
};

PNGImage::PNGImage(const PNGImage& p) : frameNumber{ p.frameNumber },
width{ p.width }, height{ p.height },
buffer{ p.buffer } {}
 void PNGImage::write() const {
     std::string exe_path = Helper::GetExePath();
     std::string exe_folder = Helper::GetFolder(exe_path);
     exe_folder += "\\";

    std::string file_name = std::string("out") + std::to_string(frameNumber) + ".png";
    exe_folder += file_name;
    std::cout << exe_folder << std::endl;
    if (lodepng::encode(exe_folder, *buffer, width, height)) {
        std::cerr << "Error: could not write PNG file!" << std::endl;
    }
}

int ch02::fig_2_28::stereo3DFrameCounter = 0;
int ch02::fig_2_28::stero3DNumImages = 0;
void ch02::fig_2_28::fig_2_28_run() {
    while (uint64_t frameNumber = getNextFrameNumber()) {
        auto left = getLeftImage(frameNumber);
        auto right = getRightImage(frameNumber);
        increasePNGChannel(left, PNGImage::redOffset, 10);
        increasePNGChannel(right, PNGImage::blueOffset, 10);
        mergePNGImages(right, left);
        right.write();
    }
}

void ch02::fig_2_28::initStereo3D(int num_images) {
    stereo3DFrameCounter = 0;
    stero3DNumImages = num_images;
}

int ch02::fig_2_28::getNextFrameNumber() {
    if (stereo3DFrameCounter < stero3DNumImages) {
        return ++stereo3DFrameCounter;
    }
    else {
        return 0;
    }
}

PNGImage ch02::fig_2_28::getLeftImage(uint64_t frameNumber) {
    return PNGImage(frameNumber, "input1.png");
}

PNGImage ch02::fig_2_28::getRightImage(uint64_t frameNumber) {
    return PNGImage(frameNumber, "input2.png");
}

void ch02::fig_2_28::increasePNGChannel(PNGImage& image, int channel_offset, int increase) {
    const int height_base = PNGImage::numChannels * image.width;
    std::vector<unsigned char>& buffer = *image.buffer;

    // Increase selected color channel by a predefined value
    for (unsigned int y = 0; y < image.height; y++) {
        const int height_offset = height_base * y;
        for (unsigned int x = 0; x < image.width; x++) {
            int pixel_offset = height_offset + PNGImage::numChannels * x + channel_offset;
            buffer[pixel_offset] = static_cast<uint8_t>(std::min(buffer[pixel_offset] + increase, 255));
        }
    }
}

void ch02::fig_2_28::mergePNGImages(PNGImage& right, const PNGImage& left) {
    const int channels_per_pixel = PNGImage::numChannels;
    const int height_base = channels_per_pixel * right.width;
    std::vector<unsigned char>& left_buffer = *left.buffer;
    std::vector<unsigned char>& right_buffer = *right.buffer;

    for (unsigned int y = 0; y < right.height; y++) {
        const int height_offset = height_base * y;
        for (unsigned int x = 0; x < right.width; x++) {
            const int pixel_offset = height_offset + channels_per_pixel * x;
            const int red_index = pixel_offset + PNGImage::redOffset;
            right_buffer[red_index] = left_buffer[red_index];
        }
    }
}

int ch02::fig_2_28::main() {
    int num_images = 3;

    initStereo3D(num_images);

    double serial_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_2_28_run();
        serial_time = (tbb::tick_count::now() - t0).seconds();
    }
    std::cout << "serial_time == " << serial_time << " seconds" << std::endl;
    return 0;
}
