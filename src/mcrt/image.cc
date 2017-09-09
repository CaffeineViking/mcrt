#include "mcrt/image.hh"

const std::vector<mcrt::Color<unsigned char>>& mcrt::Image::getPixelData() const { return pixelData; }
std::vector<mcrt::Color<unsigned char>>& mcrt::Image::getPixelData() { return pixelData; }
mcrt::Color<unsigned char>  mcrt::Image::pixel(size_t x, size_t y) const { return pixel(x, y); }
mcrt::Color<unsigned char>& mcrt::Image::pixel(size_t x, size_t y) {
    return pixelData[x + y * width];
}

double mcrt::Image::getAspectRatio() const { return width / static_cast<double>(height); }
void mcrt::Image::resize(size_t width, size_t height) {
    pixelData.resize(width*height);
    this->height = height;
    this->width = width;
}

void mcrt::Image::clear(const Color<unsigned char>& clearColor) {
    for (auto& pixel : pixelData) pixel = clearColor;
}
