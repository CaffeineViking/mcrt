#ifndef MCRT_MCRT_HH
#define MCRT_MCRT_HH

#include <glm/glm.hpp>
#include <limits>
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

struct Intersection {
    double distance;    // Distance to surface point
    glm::dvec3 normal;  // Normal of surface    
}

// Return distance from ray origin to sphere, 0 means no intersection.
Intersection sphere_intersect(const Ray&, const Sphere&);

// Returns distance from ray to triangle, 0 means no intersection.
Intersection triangle_instersect(const Ray&, const Triangle&);

// Return intersection with closes boject in the scene togheter with some values usefull for continued tracing.
Intersection object_intersection(const Ray&, const std::vector<Sphere>&, const std::vector<Triangle>&);

}
#endif
