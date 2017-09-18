#include "mcrt/scene.hh"
#include <cmath>    
#include <glm/glm.hpp>
#include <limits>
#include <vector>
#include <cmath>

namespace mcrt {

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

            Ray shadowRay{point+glm::normalize(diff)*1e-6, glm::normalize(diff)};
            Intersection r = intersect(shadowRay);
	    if(r.distance >= glm::length(diff)){
                // Light hit
		double angleFallof = std::max(0.0,glm::dot(shadowRay.direction,i.normal));
		color += l.color  * i.material.color * angleFallof;
            }
        }
        return color;
    }

}
