#include <iostream>

#include "mcrt/color.hh"
#include "mcrt/image.hh"
#include "mcrt/camera.hh"
#include "mcrt/image_export.hh"

int main(int, char**) {
    mcrt::Image renderImage { 64, 64 };
    renderImage.clear({0, 0, 0,  255});

    std::cout << "Image size: " << renderImage.getWidth() << "x" << renderImage.getHeight() << std::endl;
    std::cout << "Image aspect ratio: " << renderImage.getAspectRatio() << std::endl;

    mcrt::Color<unsigned char> firstPixelColor { renderImage.pixel(0, 0) };
    std::cout << "First pixel color: (" << static_cast<unsigned>(firstPixelColor.r) << ", "
                                        << static_cast<unsigned>(firstPixelColor.g) << ", "
                                        << static_cast<unsigned>(firstPixelColor.b) << ", "
                                        << static_cast<unsigned>(firstPixelColor.a)
                                        << ")" << std::endl;

    std::cout << "Replacing image with a nice gradient." << std::endl;

    double stepX = 255.0 / renderImage.getWidth(),
           stepY = 255.0 / renderImage.getHeight();
    for (size_t y { 0 }; y < renderImage.getHeight(); ++y) {
        for (size_t x { 0 }; x < renderImage.getWidth(); ++x) {
            renderImage.pixel(x, y) = { x*stepX, y*stepY,
                                        0, 255 };
        }
    }

    std::cout << "Scaling image with bilinear interpolation." << std::endl;
    renderImage.resize(256, 256, mcrt::Image::ResizeMethod::BILINEAR);
    std::cout << "Saving PPM image to disk..." << std::endl;
    mcrt::NetpbmImageExporter::save(renderImage, "share/render.ppm");

    // Create camera with standard settings, position is rela-
    // tive to the eye position (as are all of the operaitons)
    // but not the viewport ones (like finding pixels & stuff.
    mcrt::Camera camera { {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };

    // camera.setAspectRatio(image.getAspectRatio());
    // camera.setFieldOfView(3.141592 / 2.0);
    // camera.moveTo({ 1.0, 0.0, 1.0 });
    // camera.lookAt({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});

    glm::dvec3 eye { camera.getEyePosition() };
    for (size_t y { 0 }; y < renderImage.getHeight(); ++y) {
        for (size_t x { 0 }; x < renderImage.getWidth(); ++x) {
            glm::dvec3 pixel { camera.getPixelCenter(renderImage, x, y) };
            // or: call getPixelSamplePlane for gathering more samples for
            // the actual pixel colors. e.g. when supersampling 4x or 16x.

            // Do raytracing schenanigans over here.
            glm::dvec3 rayDirection { pixel - eye };
        }
    }

    return 0;
}
