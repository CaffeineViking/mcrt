#include <iostream>

#include "mcrt/image.hh"
#include "mcrt/image_format.hh"

int main(int, char**) {
    size_t sideLength { 1000 };
    mcrt::Image image { sideLength };
    image.clear({ 0, 0, 0, 255 });

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

    mcrt::NetpbmImageFormat imageFormat;
    imageFormat.save(image, "test.ppm");
    return 0;
}
