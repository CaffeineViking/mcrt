#ifndef MCRT_PHOTON_HH
#define MCRT_PHOTON_HH

#include <glm/glm.hpp>

namespace mcrt {
    struct Photon {
        glm::dvec3 position;
        glm::dvec3 incoming;
        glm::dvec3 color;
    };
}

#endif
