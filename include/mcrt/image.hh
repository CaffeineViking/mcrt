#ifndef MCRT_IMAGE_HH
#define MCRT_IMAGE_HH

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "mcrt/color.hh"

namespace mcrt {
    class Image {
    public:
        // Used for scaling the image up to higher resolutions.
        enum class ResizeMethod { NEAREST_NEIGHBOR, BILINEAR };


        Image(size_t length) : Image { length, length } {  }
        Image(size_t width, size_t height) : width { width }, height { height },
                                             pixelData(width*height) {  }

        void clear(const Color<double>&);
        void resize(size_t, size_t, ResizeMethod);
        Color<double>& pixel(size_t, size_t);
        Color<double>  pixel(size_t, size_t) const;

        // Filters using data from a pixel.
        template<typename F> void filter(F);
        template<typename F> void filterByColor(F);
        // Filters using data from the neighborhood of a
        // pixel. Data is orderd clockwise around pixel.
        template<typename F> void filterNeighborhood(F);

        size_t getSize() const { return width*height; }
        size_t getHeight() const { return height; }
        size_t getWidth() const { return width; }
        double getAspectRatio() const;

        // Since Color is specified in a struct, it should
        // aligned element-wise. Therefore, we can convert
        // it to a stream of bytes directly without having
        // to do any sort of conversion magic. It's bytes!
        std::vector<Color<double>>& getPixelData();
        std::vector<Color<unsigned char>> getNormalizedPixelData() const;

        // Wrapper when not changing, i.e read-only of the pixel data.
        const std::vector<Color<double>>& getPixelData() const;

        static constexpr size_t CHANNELS { 4 };
        static constexpr size_t BPP { 24 };

    private:
        // Both aren't very fast, be very careful!!!
        Image bilinearInterpolation(size_t, size_t);
        Image nearestInterpolation(size_t,  size_t);
        size_t width, height; // 24 bpp in our case.
        std::vector<Color<double>> pixelData;
    };

    template<typename F>
    void Image::filterByColor(F filter) {
        for (size_t y { 0 }; y < height; ++y)
            for (size_t x { 0 }; x < width; ++x)
                pixel(x, y) = filter(pixel(x, y));
    }

    template<typename F>
    void Image::filter(F filter) {
        for (size_t y { 0 }; y < height; ++y)
            for (size_t x { 0 }; x < width; ++x)
                pixel(x, y) = filter(x, y, width, height,
                                     pixel(x, y));
    }

    template<typename F>
    void Image::filterNeighborhood(F filter) {
        int w = width-1, h = height-1;
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                Color<double> neighborhoodColors[9];
                neighborhoodColors[0] = pixel(glm::clamp(--x, 0, w), glm::clamp(--y, 0, h));
                neighborhoodColors[1] = pixel(glm::clamp(x, 0, w),   glm::clamp(--y, 0, h));
                neighborhoodColors[2] = pixel(glm::clamp(++x, 0, w), glm::clamp(--y, 0, h));
                neighborhoodColors[3] = pixel(glm::clamp(--x, 0, w), glm::clamp(y, 0, h));
                neighborhoodColors[4] = pixel(glm::clamp(x, 0, w),   glm::clamp(y, 0, h));
                neighborhoodColors[5] = pixel(glm::clamp(++x, 0, w), glm::clamp(y, 0, h));
                neighborhoodColors[6] = pixel(glm::clamp(--x, 0, w), glm::clamp(++y, 0, h));
                neighborhoodColors[7] = pixel(glm::clamp(x, 0, w),   glm::clamp(++y, 0, h));
                neighborhoodColors[8] = pixel(glm::clamp(++x, 0, w), glm::clamp(++y, 0, h));
                pixel(x, y) = filter(x, y, width, height,
                                     neighborhoodColors);
            }
    }
}

#endif
