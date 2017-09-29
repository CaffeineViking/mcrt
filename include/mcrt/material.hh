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

        glm::dvec3 brdf(const glm::dvec3& point) const { return brdf(point, {}, {}); }
        virtual glm::dvec3 brdf(const glm::dvec3&, const glm::dvec3&, const glm::dvec3&) const = 0;
    };
}

#endif
