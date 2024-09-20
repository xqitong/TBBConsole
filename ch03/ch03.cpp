#include "stdafx.h"
#include "ch03.h"
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