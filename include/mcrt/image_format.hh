#ifndef MCRT_IMAGE_FORMAT_HH
#define MCRT_IMAGE_FORMAT_HH

#include <string>

#include "mcrt/image.hh"

namespace mcrt {
    class ImageFormat {
    public:
        virtual ~ImageFormat() = default;
        // Currently we will only support saving to disk, not loading.
        virtual void  save(const Image&, const std::string&) const = 0;
    };

    // Easy-to-parse, universal binary PPM format.
    class NetpbmImageFormat : public ImageFormat {
    public:
        // Loading is not very hard in the ppm formats, todo later.
        void save(const Image&, const std::string&) const override;
    };

    // Suckless image format with alpha-channel too.
    class FarbfeldImageFormat : public ImageFormat {
    public:
        // Loading is not very hard in the FF format, might do l8r.
        void save(const Image&, const std::string&) const override;
    };
}

#endif
