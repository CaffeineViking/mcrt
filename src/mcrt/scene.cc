#include "mcrt/scene.hh"
#include <glm/glm.hpp>
#include <limits>
#include <vector>
#include <cmath>
namespace mcrt {
    Intersection Scene::intersect(const Ray& ray) const {
        Intersection closestHit {
            std::numeric_limits<double>::max(),
            glm::dvec3(0.0), 
            {glm::dvec3(0.0),MaterialType::Diffuse,0.0}
        };

        for (const Geometry* geometry : geometries) {
            Intersection rayHit = geometry->intersect(ray);
            if (rayHit.distance > 0.0 && rayHit.distance < closestHit.distance)
                closestHit = rayHit;
        }

        return closestHit;
    }

    // Will be our resource after this...
    void Scene::add(Geometry* geometry) {
        geometries.push_back(geometry);
    }

    void Scene::add(const Light& light) {
        lights.push_back(light);
    }

    // Incomming direction and surface normal, ior is refractive index
    double Scene::fresnel(const glm::dvec3& direction, const glm::dvec3& normal, const double ior) const {
        double cosi = glm::clamp(glm::dot(direction, normal),-1.0,1.0);
        double etai = 1;
        double etat = ior;
        if(cosi > 0) {
            std::swap(etai,etat);
        }

        double sint = etai / etat * std::sqrt(std::max(0.0,1.0 - cosi * cosi));
        if(sint >= 1.0) {
            return 1;
        } 
        else {
            double cost = std::sqrt(std::max(0.0,1 - sint * sint));
            cosi = std::abs(cosi);
            double Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            double Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            return (Rs * Rs + Rp * Rp) / 2.0;
        }
    }
    
    glm::dvec3 Scene::rayTrace(const Ray& ray, const int depth = 0) const {
        glm::dvec3 rayColor { 0.0 };

        // Make sure we don't bounce forever
        if(depth >= 10)
            return rayColor;

        Intersection rayHit = intersect(ray);
        glm::dvec3 rayHitPosition { ray.origin + ray.direction * rayHit.distance };
        
        // Hit diffuse object
        if(rayHit.material.type == MaterialType::Diffuse) {
            for (const Light& lightSource : lights) {
    
                glm::dvec3 rayToLightSource = lightSource.origin - rayHitPosition;
                glm::dvec3 rayToLightNormal { glm::normalize(rayToLightSource) };
    
                Ray shadowRay { rayHitPosition + rayToLightNormal*1e-8,
                                glm::normalize(rayToLightSource) };
    
                Intersection shadowRayHit { intersect(shadowRay) };
                if (shadowRayHit.distance >= glm::length(rayToLightSource)) {
                    double lambertianFalloff { std::max(0.0, glm::dot(shadowRay.direction, rayHit.normal)) };
                    rayColor += lightSource.color * rayHit.material.color * lambertianFalloff;
                }
            }
        }
        else if(rayHit.material.type == MaterialType::Reflective) {
            // Compute reflection ray
            glm::dvec3 reflectionDir = glm::normalize((ray.direction) - 2.0 * rayHit.normal * glm::dot(ray.direction,rayHit.normal));
            Ray reflectionRay {rayHitPosition + (rayHit.normal)*1e-8 ,reflectionDir };
            rayColor += rayTrace(reflectionRay, depth + 1) * 0.9; // Adding slight falloff
        }
        else if(rayHit.material.type == MaterialType::Refractive) {
            
            // Check for total reflection
            double kr = fresnel(ray.direction, rayHit.normal, rayHit.material.ior);
            bool outside = glm::dot(ray.direction, rayHit.normal) < 0.0;
            glm::dvec3 bias = 1e-8 * rayHit.normal;
            glm::dvec3 refractionColor = glm::dvec3(0.0);
            glm::dvec3 refractionDir;
            if(kr < 1.0) {
                // Compute refraction ray
                double cosi = glm::clamp(glm::dot(rayHit.normal,ray.direction),-1.0,1.0);
                double n1 = 1.0;
                double n2 = rayHit.material.ior;
                glm::dvec3 normal = rayHit.normal;
                if(cosi < 0.0) {
                    cosi = -cosi;
                }
                else {
                    std::swap(n1,n2);   
                    normal = -normal;                 
                }

                double n = n1 / n2;
                double k = 1.0 - n * n * (1.0 - cosi * cosi);
                if(k < 0.0) {
                    refractionDir = glm::dvec3(0.0);
                }
                else {
                    refractionDir = n * ray.direction + (n * cosi - glm::sqrt(k)) * normal;
                }
                Ray refractionRay {rayHitPosition + (outside ? -bias : bias), refractionDir};
                refractionColor = rayTrace(refractionRay,depth + 1);
            }            
            // Compute reflection ray
            glm::dvec3 reflectionDir = glm::normalize((ray.direction) - 2.0 * rayHit.normal * glm::dot(ray.direction,rayHit.normal));
            Ray reflectionRay {rayHitPosition + (outside ? bias : -bias) ,reflectionDir };
            glm::dvec3 reflectionColor = rayTrace(reflectionRay, depth + 1);
            rayColor += reflectionColor * kr + refractionColor * (1.0 - kr);
        }


        return rayColor;
    }
}
