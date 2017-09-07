#include "mcrt/image_format.hh"

mcrt::Image mcrt::NetpbmImageFormat::load(const std::string&) const { return { 0, 0 }; }
void mcrt::NetpbmImageFormat::save(const Image& image, const std::string& file) const  {
    // ...
}

mcrt::Image mcrt::FarbfeldImageFormat::load(const std::string&) const { return { 0, 0 }; }
void mcrt::FarbfeldImageFormat::save(const Image& image, const std::string& file) const  {
    // ...
}

mcrt::Image mcrt::PngImageFormat::load(const std::string&) const { return { 0, 0 }; }
void mcrt::PngImageFormat::save(const Image&, const std::string&) const  { /*TODO*/ }
