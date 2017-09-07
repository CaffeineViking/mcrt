#ifndef MCRT_IMAGE_FORMAT_HH
#define MCRT_IMAGE_FORMAT_HH

#include <string>
#include <iomanip>
#include <fstream>

#include "mcrt/image.hh"

namespace mcrt {
    // The binary PPM formats.
    struct NetpbmImageFormat {
        static Image load(const std::string&);
        static void save(const Image&, const std::string&);
    };

    // Suckless Farbfeld format.
    struct FarbfeldImageFormat {
        static Image load(const std::string&);
        static void save(const Image&, const std::string&);
    };
}

#endif
