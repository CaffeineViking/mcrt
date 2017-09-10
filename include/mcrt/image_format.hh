#ifndef MCRT_IMAGE_FORMAT_HH
#define MCRT_IMAGE_FORMAT_HH

#include <string>

#include "mcrt/image.hh"

namespace mcrt {
    // We'll only allow saves for now.
    class ImageFormat : public Image {
    public:
        using Image::Image;
        virtual void  save(const std::string&) const = 0;
    };

    // Easy-to-parse, a binary image format.
    class NetpbmImage : public ImageFormat {
    public:
        using ImageFormat::ImageFormat;
        void save(const std::string&) const override;
    };

    // Simple image format with alpha-channel.
    class FarbfeldImage : public ImageFormat {
    public:
        using ImageFormat::ImageFormat;
        void save(const std::string&) const override;
    };
}

#endif
