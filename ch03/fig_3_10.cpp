#include "stdafx.h"
#include "fig_3_10.h"
int ch03::fig_3_10::stereo3DFrameCounter = 0;
int ch03::fig_3_10::stero3DNumImages = 0;
void ch03::fig_3_10::fig_3_10_run() {
    using Image = PNGImage;
    // step 1: create graph object
    tbb::flow::graph g;

    // step 2: create nodes
    tbb::flow::input_node<uint64_t> frame_no_node{ g,
        [](tbb::flow_control& fc) -> uint64_t {
            uint64_t frame_number = getNextFrameNumber();
            if (frame_number)
                return frame_number;
            else {
                fc.stop();
                return frame_number;
            }

        }
    };
    tbb::flow::function_node<uint64_t, Image> get_left_node{ g,
        /* concurrency */ tbb::flow::serial,
        [](uint64_t frame_number) -> Image {
            return getLeftImage(frame_number);
        }
    };
    tbb::flow::function_node<uint64_t, Image> get_right_node{ g,
        /* concurrency */ tbb::flow::serial,
        [](uint64_t frame_number) -> Image {
            return getRightImage(frame_number);
        }
    };
    tbb::flow::function_node<Image, Image> increase_left_node{ g,
        /* concurrency */ tbb::flow::unlimited,
        [](Image left) -> Image {
            increasePNGChannel(left, Image::redOffset, 10);
            return left;
        }
    };
    tbb::flow::function_node<Image, Image> increase_right_node{ g,
        /* concurrency */ tbb::flow::unlimited,
        [](Image right) -> Image {
            increasePNGChannel(right, Image::blueOffset, 10);
            return right;
        }
    };
    tbb::flow::join_node<std::tuple<Image, Image>, tbb::flow::tag_matching >
        join_images_node(g, [](Image left) { return left.frameNumber; },
            [](Image right) { return right.frameNumber; });
    tbb::flow::function_node<std::tuple<Image, Image>, Image> merge_images_node{ g,
        /* concurrency */ tbb::flow::unlimited,
        [](std::tuple<Image, Image> t) -> Image {
            auto& l = std::get<0>(t);
            auto& r = std::get<1>(t);
            mergePNGImages(r, l);
            return r;
        }
    };
    tbb::flow::function_node<Image> write_node{ g,
        /* concurrency */ tbb::flow::unlimited,
        [](Image img) {
            img.write();
        }
    };

    // step 3: add edges
    tbb::flow::make_edge(frame_no_node, get_left_node);
    tbb::flow::make_edge(frame_no_node, get_right_node);
    tbb::flow::make_edge(get_left_node, increase_left_node);
    tbb::flow::make_edge(get_right_node, increase_right_node);
    tbb::flow::make_edge(increase_left_node,
        tbb::flow::input_port<0>(join_images_node));
    tbb::flow::make_edge(increase_right_node,
        tbb::flow::input_port<1>(join_images_node));
    tbb::flow::make_edge(join_images_node, merge_images_node);
    tbb::flow::make_edge(merge_images_node, write_node);

    // step 4: send messages in to the graph
    frame_no_node.activate();
    // step 5: wait for graph to complete
    g.wait_for_all();
}
void ch03::fig_3_10::initStereo3D(int num_images) {
    stereo3DFrameCounter = 0;
    stero3DNumImages = num_images;
}

int ch03::fig_3_10::getNextFrameNumber() {
    if (stereo3DFrameCounter < stero3DNumImages) {
        return ++stereo3DFrameCounter;
    }
    else {
        return 0;
    }
}

PNGImage ch03::fig_3_10::getLeftImage(uint64_t frameNumber) {
    return PNGImage(frameNumber, "input1.png");
}

PNGImage ch03::fig_3_10::getRightImage(uint64_t frameNumber) {
    return PNGImage(frameNumber, "input2.png");
}

void ch03::fig_3_10::increasePNGChannel(PNGImage& image, int channel_offset, int increase) {
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

void ch03::fig_3_10::mergePNGImages(PNGImage& right, const PNGImage& left) {
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

void ch03::fig_3_10::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

int ch03::fig_3_10::main() {
    int num_images = 3;

    initStereo3D(num_images);

    warmupTBB();
    double parallel_time = 0.0;
    {
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_3_10_run();
        parallel_time = (tbb::tick_count::now() - t0).seconds();
    }

    std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
    return 0;
}
