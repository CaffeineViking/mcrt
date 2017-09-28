#ifndef MCRT_LIGHTS_HH
#define MCRT_LIGHTS_HH

#include <glm/glm.hpp>

namespace mcrt {
    struct PointLight {
        glm::dvec3 origin;
        glm::dvec3 color;
    };

    struct AreaLight {
        AreaLight() = default;
        AreaLight(glm::dvec3, glm::dvec3, glm::dvec3, glm::dvec3);

        glm::dvec3 v0;
        glm::dvec3 v1;
        glm::dvec3 v2;
        glm::dvec3 color;

        glm::dvec3 sample();
    };
}

#endif
