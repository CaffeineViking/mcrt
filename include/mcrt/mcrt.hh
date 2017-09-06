#ifndef MCRT_MCRT_HH
#define MCRT_MCRT_HH

#include <glm/glm.hpp>

namespace mcrt {    

// Material struct, stores render information about a surface type.
struct Material {
    glm::dvec3 color;
};

// Parametric form representation of a sphere
struct Sphere {
    glm::dvec3 origin;
    double radius;
    Material material;
};

struct Ray {
    glm::dvec3 origin;
    glm::dvec3 direction;
};

// 
struct Triangle {
    glm::dvec3 v1;
    glm::dvec3 v2;
    glm::dvec3 v3;
    Material material;
};

// Return distance from ray origin to sphere, -1 means no intersection.
double sphere_intersect(const Ray&, const Sphere&);

// Returns distance from ray to triangle, -1 means no intersection.
double triangle_instersect(const Ray&, const Triangle&);

}
#endif
