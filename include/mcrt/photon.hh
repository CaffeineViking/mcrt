#ifndef MCRT_PHOTON_HH
#define MCRT_PHOTON_HH

#include <iostream>
#include <glm/glm.hpp>

namespace mcrt {
    struct Photon {
        glm::dvec3 position;
        glm::dvec3 incoming;
        glm::dvec3 color;
        bool shadow;
    };

}

std::ostream& operator<<(std::ostream&,
                  const mcrt::Photon&);

#endif
