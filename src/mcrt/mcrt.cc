#include "mcrt/mcrt.hh"
#include <cmath>    
#include <glm/glm.hpp>
#include <limits>
#include <vector>
namespace mcrt{
        
    Material::Material(const glm::dvec3& c):
    _color{c}
    {}
        
    Geometry::Geometry(const Material& m): 
    _material{m}
    {}
        
    Sphere::Sphere(const glm::dvec3 o, double r, const Material& m):
    Geometry(m),    
    _origin{o},
    _radius{r}
    {}
            
    Triangle::Triangle(const glm::dvec3& v1,const glm::dvec3& v2,const glm::dvec3& v3, const Material& m):
    Geometry(m),
    _v1{v1},
    _v2{v2},
    _v3{v3}
    {} 
                
    // Return distance from ray origin to sphere, distance = 0 means no intersection.
    Intersection Sphere::intersect(const Ray& ray) {
        Intersection result {0.0,glm::dvec3(0.0)};
        glm::dvec3 a = ray.origin - _origin;
        double b = glm::dot(a,ray.direction);
        double c = glm::dot(a,a) - (_radius * _radius);
        double d = b * b - c;
        if(d > 1e-8) {
            double dist = -b - sqrt(d);
            result.distance = dist;
            result.normal = glm::normalize((ray.origin + ray.direction) - _origin);
            return result;        
        }
        return result;
    }

    // Returns distance from ray to triangle, 0 means no intersection.
    Intersection Triangle::intersect(const Ray& ray) {
        glm::dvec3 e1 = _v2 - _v1;
        glm::dvec3 e2 = _v3 - _v1;
        
        // Surface normal I think
        glm::dvec3 pvec = glm::cross(ray.direction,e2);
        double det = glm::dot(e1,pvec);
        
        Intersection result {0.0,pvec};    
        
        if(det < 1e-8 && det > -1e-8) {
            return result;
        }
        
        double inv_det = 1.0 / det;
        glm::dvec3 tvec = ray.origin - _v1;
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
    // Intersection object_intersection(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Triangle>& triangles) {
    //     Intersection result
    //     {
    //         std::numeric_limits<double>::max(),            
    //         glm::dvec3(0.0)
    //     };
        
    //     for(const Sphere& s: spheres) {
    //         Intersection i = sphere_intersect(ray,s);
    //         if(i.distance > 0.0 && i.distance < result.distance) {
    //             result = i;
    //         }
    //     }
        
    //     for(const Triangle& t: triangles) {
    //         Intersection i = triangle_instersect(ray,t);
    //         if(i.distance > 0.0 && i.distance < result.distance) {
    //             result = i;
    //         }
    //     }
    //     return result;
    // }
}