#include "mcrt/image_format.hh"

void mcrt::NetpbmImageFormat::save(const Image& image, const std::string& file) const  {
    std::ofstream fileStream { file, std::ios::binary };
    // Write the format magic data, as: https://en.wikipedia.org/wiki/Netpbm_format.
    fileStream << "P6 " <<  image.getWidth() << " " << image.getHeight() << " 255 ";
    for (const auto& pixelData : image.getPixelData())
        fileStream.write((const char*) pixelData, 3);
}

void mcrt::FarbfeldImageFormat::save(const Image& image, const std::string& file) const  {
    // ...
}

void mcrt::PngImageFormat::save(const Image&, const std::string&) const {
    // ...
}
