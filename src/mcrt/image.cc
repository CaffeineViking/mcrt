#include "mcrt/image.hh"

mcrt::Color<unsigned char>  mcrt::Image::pixel(size_t x, size_t y) const { return pixel(x, y); }
mcrt::Color<unsigned char>& mcrt::Image::pixel(size_t x, size_t y) {
    return pixelData[x + y * width];
}

void mcrt::Image::resize(size_t width, size_t height) {
    pixelData.resize(width*height);
    this->height = height;
    this->width = width;
}

void mcrt::Image::clear(const Color<unsigned char>& clearColor) {
    for (auto& pixel : pixelData) pixel = clearColor;
}
