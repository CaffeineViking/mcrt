#include "mcrt/mcrt.hh"
#include <glm/glm.hpp>
#include <cmath>
namespace mcrt{

// Return distance from ray origin to sphere, distance = 0 means no intersection.
Intersection sphere_intersect(const Ray& ray, const Sphere& sphere) {
    Intersection result {0.0,glm::dvec3(0.0)};
    glm::dvec3 a = ray.origin - sphere.origin;
    double b = glm::dot(a,ray.direction);
    double c = glm::dot(a,a) - (sphere.radius * sphere.radius);
    double d = b * b - c;
    if(det > 1e-8) {
        double dist = -b - sqrt(d);
        result.distance = dist;
        result.normal = glm::normalize((ray.origin + origin.direction) - sphere.origin);
        return result;        
    }
    return result;
}

// Returns distance from ray to triangle, 0 means no intersection.
Intersection triangle_instersect(const Ray& ray, const Triangle& triangle) {
    glm::dvec3 e1 = triangle.v2 - triangle.v1;
    glm::dvec3 e2 = triangle.v3 - triangle.v1;

    // Surface normal I think
    glm::dvec3 pvec = glm::cross(ray.direction,e2);
    double det = glm::dot(e1,pvec);

    Intersection result {0.0,pvec};    
    
    if(det < 1e-8 && det > -1e-8) {
        return result;
    }

    double inv_det = 1.0 / det;
    glm::dvec3 tvec = ray.origin - triangle.v1;
    double u = glm::dot(tvec,pvec) * inv_det;
    if(u < 0 || u > 1) {
        return result;
    }

    glm::dvec3 qvec = glm::cross(tvec,e1);
    double v = glm::dot(ray.direction, qvec) * inv_det;
    if(v < 0.0 || u + v > 1.0) {
        return result;
    }

    result.distance = glm::dot(e2,qvec) * inv_det;
    return result;
}

// Return intersection with closes boject in the scene togheter with some values usefull for continued tracing.
Intersection object_intersection(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Triangle>& triangles) {
    Intersection result
        {
            std::numeric_limits<double>::max()            
            glm::dvec3(0.0),
        };

    for(const Sphere& s: spheres) {
        Intersection i = sphere_intersect(ray,s);
        if(i.distance > 0.0 && i.distance < result.distance) {
            result = i;
        }
    }

    for(const Triangle& t: triangles) {
        Intersection i = triangle_instersect(ray,t);
        if(i.distance > 0.0 && i.distance < result.distance) {
            result = i;
        }
    }
    return result;
}

}