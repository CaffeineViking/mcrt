#ifndef MCRT_IMAGE_EXPORT_HH
#define MCRT_IMAGE_EXPORT_HH

#include <string>

#include "mcrt/image.hh"

namespace mcrt {
    // We currently only support save.
    class ImageExport : public Image {
    public:
        using Image::Image;
        // TODO:  add loading later if we want textures.
        virtual void save(const std::string&) const = 0;
    };

    // Binary PPM image format, standardized and easy.
    class NetpbmImage : public ImageExport {
    public:
        using ImageExport::ImageExport;
        // P6 <width> <height> <max-number> R8 G8 B8.
        void save(const std::string&) const override;
    };

    // Suckless 16-bit data RGBA image format.
    class FarbfeldImage : public ImageExport {
    public:
        using ImageExport::ImageExport;
        // farbfeld <width> <height> R16 G16 B16 A16.
        void save(const std::string&) const override;
    };
}

#endif
