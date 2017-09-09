#include <iostream>

#include "mcrt/image.hh"
#include "mcrt/image_format.hh"

int main(int, char**) {
    size_t sideLength { 8 };
    mcrt::Image image { sideLength };
    // Apply black color to pixels...
    image.clear({ 0, 0, 0, 255 });

    // Assign increasing colors to (x, y) --> (r, g) to test.
    double stepSize { 255 / static_cast<double>(sideLength) };
    for (size_t y { 0 }; y < image.getHeight(); ++y) {
        for (size_t x { 0 }; x < image.getWidth(); ++x) {
            image.pixel(x, y) += {
                static_cast<unsigned char>(x * stepSize),
                static_cast<unsigned char>(y * stepSize),
                255,
                0
            };
        }
    }

    // Resize the images up or down using bi-linear filtering.
    image.resize(512, 512, mcrt::Image::ResizeMethod::BILINEAR);

    mcrt::FarbfeldImageFormat imageFormat;
    imageFormat.save(image, "share/x.ff");
    return 0;
}
