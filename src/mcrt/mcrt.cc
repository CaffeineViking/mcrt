#include "mcrt/mcrt.hh"
#include <cmath>    
#include <glm/glm.hpp>
#include <limits>
#include <vector>
#include <cmath>
#include <iostream>

namespace mcrt{

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
            
    Material Geometry::getMaterial() const {
        return _material;
    }

    // Return distance from ray origin to sphere, distance = 0 means no intersection.
    Intersection Sphere::intersect(const Ray& ray) const{
        Intersection result {0,glm::dvec3(0.0),{glm::dvec3(0.0)}};
        
        double t0,t1;
        glm::dvec3 L = _origin - ray.origin;
        double tca = glm::dot(L,ray.direction);
        if(tca < 0) {
            return result;            
        }

        double d2 = glm::dot(L,L) - tca * tca;
        if( d2 > _radius*_radius) {
            return result;
        }

        double thc = std::sqrt(_radius*_radius -d2);
        t0 = tca - thc;
        t1 = tca + thc;

        if(t0 > t1) {
            double tmp = t0;
            t0 = t1;
            t1 = tmp;
        }

        if(t0 < 0){
            t0 = t1;
            if( t0 < 0) {
                return result;
            }
        }

        result.distance = t0;
        result.material = _material;
        result.normal = glm::normalize((ray.origin + ray.direction * t0) - _origin);
        return result;
    }

    // Returns distance from ray to triangle, 0 means no intersection.
    Intersection Triangle::intersect(const Ray& ray) const {
        glm::dvec3 e1 = _v2 - _v1;
        glm::dvec3 e2 = _v3 - _v1;
        
        // Surface normal I think
        glm::dvec3 pvec = glm::cross(ray.direction,e2);
        double det = glm::dot(e1,pvec);
        
        Intersection result {0,glm::normalize(pvec),{glm::dvec3(0.0)}};    
        
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
        result.material = _material;
        return result;
    }

    Intersection Scene::intersect(const Ray& ray) const{
        Intersection result
        {
            std::numeric_limits<double>::max(),            
            glm::dvec3(0.0),
            {glm::dvec3(0.0)}
        };

        for(const Geometry* g: _geometry){
            Intersection i = g->intersect(ray);
            if(i.distance > 0.0 && i.distance < result.distance){
                result = i;
            }
        }
        return result;
    }    

    void Scene::add(const Geometry& g) {
        _geometry.push_back(&g);
    }

    void Scene::add(Light light) {
        _lights.push_back(light);
    }
    
    glm::dvec3 Scene::rayTrace(const Ray& ray) {
        glm::dvec3 color(0.0);
        Intersection i = intersect(ray);
        for(Light l: _lights){
            glm::dvec3 point = ray.origin + ray.direction * i.distance;
            
            glm::dvec3 diff = l.origin - point;

            Ray shadowRay{point, glm::normalize(diff)};
            Intersection r = intersect(shadowRay);
            if(r.distance < 1e-8) {
                // Light hit
                double angleFallof = std::max(0.0,glm::dot(shadowRay.direction,i.normal));                
                color = l.color * r.material.color;// * angleFallof;
            } 
            else if(r.distance >= glm::length(diff)){
                // Light hit
                 double angleFallof = std::max(0.0,glm::dot(shadowRay.direction,i.normal));                
                color = l.color  * i.material.color ;//* angleFallof;
            }
        }
        return color;
    }

    
}