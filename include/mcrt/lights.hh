#ifndef MCRT_LIGHTS_HH
#define MCRT_LIGHTS_HH

namespace mcrt {
    struct PointLight {
        glm::dvec3 origin;
        glm::dvec3 color;
    };
}

#endif
