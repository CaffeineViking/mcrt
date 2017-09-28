#include "mcrt/lights.hh"
#include <random>

namespace mcrt {
    Light::Light(glm::dvec3 color) : color(color) {};
    Light::~Light() {}

    PointLight::PointLight(glm::dvec3 origin, glm::dvec3 color) : Light(color), origin(origin) {}
 
    AreaLight::AreaLight(glm::dvec3 v0, glm::dvec3 v1, glm::dvec3 v2, glm::dvec3 color) : Light(color), v0(v0), v1(v1), v2(v2) {}
 
    glm::dvec3 AreaLight::sample() {     
        double u;
        double v;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0,1);

        do {
            u = dist(gen);
            v = dist(gen);
        } while (u + v >= 1);

        return (1 - u - v)*v0 + u*v1 + v*v2;
    }
}
