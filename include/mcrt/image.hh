#ifndef MCRT_IMAGE_HH
#define MCRT_IMAGE_HH

#include <string>
#include <vector>

#include "mcrt/color.hh"

namespace mcrt {
    class Image {
    public:
        Image(size_t width, size_t height) : width { width }, height { height },
                                             pixelData(width*height) {  }

        void resize(size_t, size_t);
        void clear(const Color<unsigned char>&);

        Color<unsigned char>& pixel(size_t, size_t);
        Color<unsigned char>  pixel(size_t, size_t) const;

        size_t getSize() const { return width*height; }
        size_t getHeight() const { return height; }
        size_t getWidth() const { return width; }

        // Wrapper for saving to disk.
        template<typename ImageFormat>
        void save(const std::string&) const;

        static constexpr size_t CHANNELS { 4 };
        static constexpr size_t BPP { 24 };

    private:
        size_t width, height; // 24 bpp in our case.
        std::vector<Color<unsigned char>> pixelData;
    };

    template<typename ImageFormat>
    void Image::save(const std::string& file) const {
        ImageFormat::save(*this, file);
    }
}

#endif
