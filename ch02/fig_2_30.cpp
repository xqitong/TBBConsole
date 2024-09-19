#include "stdafx.h"
#include "fig_2_30.h"
int ch02::fig_2_30::stereo3DFrameCounter = 0;
int ch02::fig_2_30::stero3DNumImages = 0;
void ch02::fig_2_30::fig_2_30_run() {
    using Image = PNGImage;
    using ImagePair = std::pair<PNGImage, PNGImage>;
    tbb::parallel_pipeline(
        /* tokens */ 8,
        /* make the left image filter */
        tbb::make_filter<void, Image>(
            /* filter type */ tbb::filter_mode::serial_in_order,
            [&](tbb::flow_control& fc) -> Image {
                if (uint64_t frame_number = getNextFrameNumber()) {
                    return getLeftImage(frame_number);
                }
                else {
                    fc.stop();
                    return Image{};
                }
            }) &
        tbb::make_filter<Image, ImagePair>(
            /* filter type */ tbb::filter_mode::serial_in_order,
            [&](Image left) -> ImagePair {
                return ImagePair(left, getRightImage(left.frameNumber));
            }) &
                tbb::make_filter<ImagePair, ImagePair>(
                    /* filter type */ tbb::filter_mode::parallel,
                    [&](ImagePair p) -> ImagePair {
                        increasePNGChannel(p.first, Image::redOffset, 10);
                        return p;
                    }) &
                tbb::make_filter<ImagePair, ImagePair>(
                    /* filter type */ tbb::filter_mode::parallel,
                    [&](ImagePair p) -> ImagePair {
                        increasePNGChannel(p.second, Image::blueOffset, 10);
                        return p;
                    }) &
                        tbb::make_filter<ImagePair, Image>(
                            /* filter type */ tbb::filter_mode::parallel,
                            [&](ImagePair p) -> Image {
                                mergePNGImages(p.second, p.first);
                                return p.second;
                            }) &
                        tbb::make_filter<Image, void>(
                            /* filter type */ tbb::filter_mode::parallel,
                            [&](Image img) {
                                img.write();
                            })
                                );
}
void ch02::fig_2_30::initStereo3D(int num_images) {
     stereo3DFrameCounter = 0;
     stero3DNumImages = num_images;
 }
int ch02::fig_2_30::getNextFrameNumber() {
    if (stereo3DFrameCounter < stero3DNumImages) {
        return ++stereo3DFrameCounter;
    }
    else {
        return 0;
    }
}
PNGImage ch02::fig_2_30::getLeftImage(uint64_t frameNumber) {
    return PNGImage(frameNumber, "input1.png");
}
PNGImage ch02::fig_2_30::getRightImage(uint64_t frameNumber) {
    return PNGImage(frameNumber, "input2.png");
}
void ch02::fig_2_30::increasePNGChannel(PNGImage& image, int channel_offset, int increase) {
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
void ch02::fig_2_30::mergePNGImages(PNGImage& right, const PNGImage& left) {
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
void ch02::fig_2_30::warmupTBB() {
     tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
         tbb::tick_count t0 = tbb::tick_count::now();
         while ((tbb::tick_count::now() - t0).seconds() < 0.01);
         });
 }
int ch02::fig_2_30::main() {
      int num_images = 3;

      initStereo3D(num_images);

      warmupTBB();
      double parallel_time = 0.0;
      {
          tbb::tick_count t0 = tbb::tick_count::now();
          fig_2_30_run();
          parallel_time = (tbb::tick_count::now() - t0).seconds();
      }
      std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
      return 0;
  }
