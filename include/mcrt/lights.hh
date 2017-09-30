#ifndef MCRT_LIGHTS_HH
#define MCRT_LIGHTS_HH

#include <glm/glm.hpp>
#include "ray.hh"

namespace mcrt {
    struct Light {
        enum Type {
            PointLight,
            AreaLight
        };
        Light(glm::dvec3);
        virtual ~Light() = 0;
        glm::dvec3 color;
    };

    struct PointLight : public Light {
        PointLight(glm::dvec3, glm::dvec3);
        ~PointLight() {};
        glm::dvec3 origin;
    };

    struct AreaLight : public Light {
        AreaLight(glm::dvec3, glm::dvec3, glm::dvec3, glm::dvec3);
        ~AreaLight() {};
        glm::dvec3 v0;
        glm::dvec3 v1;
        glm::dvec3 v2;
        glm::dvec3 normal;
        double area;

        glm::dvec3 sample();
        Ray::Intersection intersect(const Ray&) const;
    };
}

#endif
