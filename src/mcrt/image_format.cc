#include "mcrt/image_format.hh"

mcrt::Image mcrt::NetpbmImageFormat::load(const std::string& file) {
    return mcrt::Image { 0, 0 };
}

void mcrt::NetpbmImageFormat::save(const Image& image, const std::string& file) {
}

mcrt::Image mcrt::FarbfeldImageFormat::load(const std::string& file) {
    return mcrt::Image { 0, 0 };
}

void mcrt::FarbfeldImageFormat::save(const Image& image, const std::string& file) {
}
