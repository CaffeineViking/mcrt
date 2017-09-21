#include "mcrt/image.hh"

#include <cmath>
#include <iostream>
#include <algorithm>

#include "mcrt/interpolants.hh"

std::vector<mcrt::Color<unsigned char>> mcrt::Image::getNormalizedPixelData() const {
    const double maxColorR = std::max_element(std::begin(pixelData), std::end(pixelData),
                                              [](const Color<double>& c1, const Color<double>& c2) -> bool {
                                                  return c1.r < c2.r;
                                              })->r;
    const double maxColorG = std::max_element(std::begin(pixelData), std::end(pixelData),
                                              [](const Color<double>& c1, const Color<double>& c2) -> bool {
                                                  return c1.g < c2.g;
                                              })->g;
    const double maxColorB = std::max_element(std::begin(pixelData), std::end(pixelData),
                                              [](const Color<double>& c1, const Color<double>& c2) -> bool {
                                                  return c1.b < c2.b;
                                              })->b;

    const double maxValue = std::max(std::max(maxColorR, maxColorG), maxColorB);

    std::vector<Color<unsigned char>> normalizedPixelData;
    std::transform(std::begin(pixelData), std::end(pixelData), std::back_inserter(normalizedPixelData),
        [&maxValue](const Color<double> & color) -> Color<unsigned char> {
            Color<double> n = color/maxValue;
            Color<unsigned char> r ((unsigned char) (n.r * 255),
                                    (unsigned char) (n.g * 255),
                                    (unsigned char) (n.b * 255),
                                    (unsigned char) 255);
            return r;
        });

    return normalizedPixelData;
}

const std::vector<mcrt::Color<double>>& mcrt::Image::getPixelData() const { return pixelData; }
std::vector<mcrt::Color<double>>& mcrt::Image::getPixelData() { return pixelData; }
mcrt::Color<double>  mcrt::Image::pixel(size_t x, size_t y) const { return pixel(x, y); }
mcrt::Color<double>& mcrt::Image::pixel(size_t x, size_t y) {
    return pixelData[x + y * width];
}

double mcrt::Image::getAspectRatio() const { return width / static_cast<double>(height); }
void mcrt::Image::resize(size_t width, size_t height, ResizeMethod method = ResizeMethod::NEAREST_NEIGHBOR) {
    switch (method) {
    // Good to reduce the amount of rays to send and still get pretty high-resolution images.
    case ResizeMethod::NEAREST_NEIGHBOR: *this = nearestInterpolation(width, height); break;
    case ResizeMethod::BILINEAR: *this = bilinearInterpolation(width, height); break;
    default: *this = nearestInterpolation(width, height);
    }

    this->height = height;
    this->width = width;
}

void mcrt::Image::clear(const Color<double>& clearColor) {
    for (auto& pixel : pixelData) pixel = clearColor;
}

mcrt::Image mcrt::Image::bilinearInterpolation(size_t width, size_t height) {
    double widthRatio  { this->width / static_cast<double>(width) };
    double heightRatio { this->height / static_cast<double>(height) };
    Image interpolatedImage { width, height };

    for (size_t y { 0 }; y < height; ++y) {
        for (size_t x { 0 }; x < width; ++x) {
            // Scale from interpolated size to
            // original img coordinate system.
            double scaledX { x * widthRatio },
                   scaledY { y * heightRatio };

            // Pick four pixels near samples.
            size_t tli = std::floor(scaledX),
                   tlj = std::floor(scaledY);
            size_t tri = tli + 1, trj = tlj;
            size_t bri = tli + 1, brj = tlj + 1;
            size_t bli = tli, blj = tlj + 1;

            // Find weights of the contributions of the pixels.
            double iDist = 1.0 - (tri - scaledX) / (tri - tli),
                   jDist = 1.0 - (blj - scaledY) / (blj - tlj);

            // Handle cases when pixel are on edges.
            if (tli == this->width  - 1)  tri = tli;
            if (bli == this->width  - 1)  bri = bli;
            if (tlj == this->height - 1)  blj = tlj;
            if (trj == this->height - 1)  brj = trj;

            // Pick the actual colors we interpolate.
            Color<double> tl { pixel(tli, tlj) },
                                 tr { pixel(tri, trj) },
                                 br { pixel(bri, brj) },
                                 bl { pixel(bli, blj) };

            // Finally, interpolate using the four pixels with the given weights.
            interpolatedImage.pixel(x, y) = interpolants::bilinear(tl, tr, br, bl,
                                                                   iDist,  jDist);
        }
    }

    return interpolatedImage;
}

mcrt::Image mcrt::Image::nearestInterpolation(size_t width, size_t height)  {
    double widthRatio  { this->width / static_cast<double>(width) };
    double heightRatio { this->height / static_cast<double>(height) };

    Image interpolatedImage { width, height };
    for (size_t y { 0 }; y < height; ++y) {
        for (size_t x { 0 }; x < width; ++x) {
            // Scale from interpolated size to
            // original img coordinate system.
            double scaledX { x * widthRatio },
                   scaledY { y * heightRatio };

            // Just pick some pixel there.
            size_t i = std::floor(scaledX),
                   j = std::floor(scaledY);

            // Copy over pixel from old to a new image.
            interpolatedImage.pixel(x, y) = pixel(i, j);
        }
    }

    return interpolatedImage;
}
