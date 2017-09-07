#include "mcrt/mcrt.hh"
#include <glm/glm.hpp>

namespace mcrt{

// Return distance from ray origin to sphere, -1 means no intersection.
double sphere_intersect(const Ray& ray, const Sphere& sphere) {
    glm::dvec3 a = ray.origin - sphere.origin;
}

// Returns distance from ray to triangle, 0 means no intersection.
double triangle_instersect(const Ray& ray, const Triangle& triangle) {
    glm::dvec3 e1 = triangle.v2 - triangle.v1;
    glm::dvec3 e2 = triangle.v3 - triangle.v1;

    glm::dvec3 pvec = glm::cross(ray.direction,e2);
    double det = glm::dot(e1,pvec);

    if(det < 1e-8 && det > -1e-8) {
        return 0.0;
    }

    double inv_det = 1.0 / det;
    glm::dvec3 tvec = ray.origin - triangle.v1;
    double u = glm::dot(tvec,pvec) * inv_det;
    if(u < 0 || u > 1) {
        return 0.0;
    }

    glm::dvec3 qvec = glm::cross(tvec,e1);
    double v = glm::dot(ray.direction, qvec) * inv_det;
    if(v < 0.0 || u + v > 1.0) {
        return 0.0;
    }

    return glm::dot(e2,qvec) * inv_det;
}


}