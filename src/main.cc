#include <iostream>
#include "mcrt/mcrt.hh"

int main(int, char**) {
    std::cout << "Hello, world!" << std::endl;

              
    mcrt::Material dummyMaterial{glm::dvec3(0.0)};
    mcrt::Ray ray{glm::dvec3(0.0), glm::dvec3(0.0,0.0,1.0)};
    mcrt::Triangle triangle{
        glm::dvec3( 5.0, 5.0, 5.0),
        glm::dvec3(-5.0, 5.0, 5.0),
        glm::dvec3( 5.0,-5.0, 5.0),
        dummyMaterial};
    double distance = triangle.intersect(ray).distance;
    std::cout << distance << std::endl;

    mcrt::Sphere sphere{glm::dvec3(0.0,0.0,4.0),2.0, dummyMaterial};
    distance = sphere.intersect(ray).distance;
    std::cout << distance << std::endl;

    return 0;
}
