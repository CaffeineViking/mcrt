#include "mcrt/lights.hh"
#include <random>

namespace mcrt {
    AreaLight::AreaLight(glm::dvec3 v0, glm::dvec3 v1, glm::dvec3 v2, glm::dvec3 color) : v0(v0), v1(v1), v2(v2), color(color) {}
 
    glm::dvec3 AreaLight::sample() {     
        //double A = 0.5*glm::length(glm::cross(v1 - v0, v2 - v0));
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
