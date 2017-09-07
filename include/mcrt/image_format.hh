#ifndef MCRT_IMAGE_FORMAT_HH
#define MCRT_IMAGE_FORMAT_HH

#include <string>
#include <fstream>

#include "mcrt/image.hh"

namespace mcrt {
    class ImageFormat {
    public:
        virtual ~ImageFormat() = default;
        virtual Image load(const std::string&) const = 0;
        virtual void  save(const Image&, const std::string&) const = 0;
    };

    // Easy-to-parse, universal binary PPM format.
    class NetpbmImageFormat : public ImageFormat {
    public:
        Image load(const std::string&) const override;
        void save(const Image&, const std::string&) const override;
    };

    // Suckless image format with alpha-channel too.
    class FarbfeldImageFormat : public ImageFormat {
    public:
        Image load(const std::string&) const override;
        void save(const Image&, const std::string&) const override;
    };

    // Lossless image format with RGBA support.
    class PngImageFormat : public ImageFormat {
    public:
        Image load(const std::string&) const override;
        void save(const Image&, const std::string&) const override;
    };
}

#endif
