#include <iostream>

#include "mcrt/camera.hh"
#include "mcrt/image_export.hh"
#include <glm/gtx/string_cast.hpp>

int main(int, char**) {
    mcrt::Image image { 16 };
    mcrt::Camera camera { { 0.0, 0.0, 0.0 }, {  0.0, 0.0, -1.0 } };
    camera.moveTo({ 0.0, 1.0, -3.0 });
    mcrt::Camera::SamplingPlane p { camera.getPixelSamplingPlane(image, 0, 0) };
    std::cout << glm::to_string(p.corners[0]) << " "
              << glm::to_string(p.corners[1]) << " "
              << glm::to_string(p.corners[2]) << " "
              << glm::to_string(p.corners[3]) << " " << std::endl;
    std::cout << glm::to_string(camera.getPixelCenter(image, 0, 0)) << std::endl;
    std::cout << camera << std::endl;
    // Why two formats? One has alpha support and one not :)
    // mcrt::NetpbmImageExporter::save(image,  "hello.ppm");
    // mcrt::FarbfeldImageExporter::save(image, "hello.ff");
    return 0;
}
