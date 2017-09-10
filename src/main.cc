#include <iostream>

#include "mcrt/camera.hh"

int main(int, char**) {
    mcrt::Camera camera { { 0.0, 0.0, 0.0 }, {  0.0, 0.0, -1.0 } };
    std::cout << camera << std::endl;
    return 0;
}
