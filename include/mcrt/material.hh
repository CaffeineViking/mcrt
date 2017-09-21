#ifndef MCRT_MATERIAL_HH
#define MCRT_MATERIAL_HH

#include <glm/glm.hpp>

namespace mcrt {
    struct Material {
        enum Type {
            Diffuse     = 0,
            Reflective  = 1,
            Refractive  = 2
        };

        glm::dvec3 color;
        Type type;
        double refractionIndex;
    };
}

#endif
