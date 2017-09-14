#ifndef MCRT_IMAGE_EXPORT_HH
#define MCRT_IMAGE_EXPORT_HH

#include <string>

#include "mcrt/image.hh"

namespace mcrt {
    // Self-aware export.
    class ImageExporter {
    public:
        // Chooses correct format based on given extension.
        static void save(const Image&, const std::string&);
    };
    // Binary PPM Image Format.
    class NetpbmImageExporter {
    public:
        // P6 <width> <height> <max-channel> [<R8><G8><B8>]
        static void save(const Image&, const std::string&);
    };

    // Suckless 64bpp RGBA image.
    class FarbfeldImageExporter {
    public:
        // farbfeld <width> <height> [<R16><G16><B16><A16>]
        static void save(const Image&, const std::string&);
    };

    // Lossless RGBA format.
    class PngImageExporter {
    public:
        // We're using the standard libPNG implementations.
        static void save(const Image&, const std::string&);
    };
}

#endif
