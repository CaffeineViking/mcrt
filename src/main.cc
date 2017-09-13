#include <iostream>

#include "mcrt/camera.hh"
#include "mcrt/image_export.hh"
#include <glm/gtx/string_cast.hpp>

int main(int, char**) {
    mcrt::Image image { 16 };
    mcrt::Camera camera { { 0.0, 0.0, 0.0 }, {  0.0, 0.0, -1.0 } };
    camera.setAspectRatio(16.0 / 9.0);
    std::cout << camera << std::endl;
    return 0;
}
