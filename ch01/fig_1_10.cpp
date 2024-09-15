#include "stdafx.h"
#include "fig_1_10.h"

using namespace ch01;
 void ch01::fig_1_10::fig_1_10_run(const std::vector<ImagePtr>& image_vector) {
    const double tint_array[] = { 0.75, 0, 0 };

    tbb::flow::graph g;
    int i = 0;
    tbb::flow::input_node<ImagePtr> src(g, [&](tbb::flow_control& fc) -> ImagePtr
        {
            if (i < image_vector.size())
            {
                return image_vector[i++];
            }
            else
            {
                fc.stop();
                return nullptr;
            }
        });

    tbb::flow::function_node<ImagePtr, ImagePtr> gamma(g,
        tbb::flow::unlimited,
        [](ImagePtr img) -> ImagePtr {
            return applyGamma(img, 1.4);
        }
    );

    tbb::flow::function_node<ImagePtr, ImagePtr> tint(g,
        tbb::flow::unlimited,
        [tint_array](ImagePtr img) -> ImagePtr {
            return applyTint(img, tint_array);
        }
    );

    tbb::flow::function_node<ImagePtr> write(g,
        tbb::flow::unlimited,
        [](ImagePtr img) {
            writeImage(img);
        }
    );

    tbb::flow::make_edge(src, gamma);
    tbb::flow::make_edge(gamma, tint);
    tbb::flow::make_edge(tint, write);
    src.activate();
    g.wait_for_all();
}

 ImagePtr ch01::fig_1_10::applyGamma(ImagePtr image_ptr, double gamma) {
    auto output_image_ptr =
        std::make_shared<ch01::Image>(image_ptr->name() + "_gamma",
            ch01::IMAGE_WIDTH, ch01::IMAGE_HEIGHT);
    auto in_rows = image_ptr->rows();
    auto out_rows = output_image_ptr->rows();
    const int height = in_rows.size();
    const int width = in_rows[1] - in_rows[0];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const ch01::Image::Pixel& p = in_rows[i][j];
            double v = 0.3 * p.bgra[2] + 0.59 * p.bgra[1] + 0.11 * p.bgra[0];
            double res = pow(v, gamma);
            if (res > ch01::MAX_BGR_VALUE) res = ch01::MAX_BGR_VALUE;
            out_rows[i][j] = ch01::Image::Pixel(res, res, res);
        }
    }
    return output_image_ptr;
}

 ImagePtr ch01::fig_1_10::applyTint(ImagePtr image_ptr, const double* tints) {
    auto output_image_ptr =
        std::make_shared<ch01::Image>(image_ptr->name() + "_tinted",
            ch01::IMAGE_WIDTH, ch01::IMAGE_HEIGHT);
    auto in_rows = image_ptr->rows();
    auto out_rows = output_image_ptr->rows();
    const int height = in_rows.size();
    const int width = in_rows[1] - in_rows[0];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const ch01::Image::Pixel& p = in_rows[i][j];
            std::uint8_t b = (double)p.bgra[0] +
                (ch01::MAX_BGR_VALUE - p.bgra[0]) * tints[0];
            std::uint8_t g = (double)p.bgra[1] +
                (ch01::MAX_BGR_VALUE - p.bgra[1]) * tints[1];
            std::uint8_t r = (double)p.bgra[2] +
                (ch01::MAX_BGR_VALUE - p.bgra[2]) * tints[2];
            out_rows[i][j] =
                ch01::Image::Pixel(
                    (b > ch01::MAX_BGR_VALUE) ? ch01::MAX_BGR_VALUE : b,
                    (g > ch01::MAX_BGR_VALUE) ? ch01::MAX_BGR_VALUE : g,
                    (r > ch01::MAX_BGR_VALUE) ? ch01::MAX_BGR_VALUE : r
                );
        }
    }
    return output_image_ptr;
}

void ch01::fig_1_10::writeImage(ImagePtr image_ptr) {
    image_ptr->write((image_ptr->name() + ".bmp").c_str());
}

int ch01::fig_1_10::main() {
    std::vector<ImagePtr> image_vector;

    for (int i = 2000; i < 20000000; i *= 10)
        image_vector.push_back(ch01::makeFractalImage(i));

    // warmup the scheduler
    oneapi::tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
    while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });

    tbb::tick_count t0 = tbb::tick_count::now();
    fig_1_10_run(image_vector);
    std::cout << "Time : " << (tbb::tick_count::now() - t0).seconds()
        << " seconds" << std::endl;
    return 0;
}
