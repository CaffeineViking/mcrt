#include "mcrt/scene.hh"

#include <glm/glm.hpp>
#include <limits>
#include <vector>
#include <cmath>

namespace mcrt {
    Ray::Intersection Scene::intersect(const Ray& ray) const {
        Ray::Intersection closestHit {
            std::numeric_limits<double>::max(),
            glm::dvec3(0.0),
            {glm::dvec3(0.0),Material::Type::Diffuse,0.0}
        };

        for (const Geometry* geometry : geometries) {
            Ray::Intersection rayHit = geometry->intersect(ray);
            if (rayHit.distance > 0.0 && rayHit.distance < closestHit.distance)
                closestHit = rayHit;
        }

        // Should we check intersections with lights?
        /*
        for (Light* light : lights) {
            AreaLight* areaLight = dynamic_cast<AreaLight*>(light);
            if (areaLight) {
                Ray::Intersection rayHit = areaLight->intersect(ray);
                if (rayHit.distance > 0.0 && rayHit.distance < closestHit.distance)
                    closestHit = rayHit;
            }
        }
        */

        return closestHit;
    }

    // Will be our resource after this...
    void Scene::add(Geometry* geometry) {
        geometries.push_back(geometry);
    }

    void Scene::add(Light* light) {
        lights.push_back(light);
    }

    glm::dvec3 Scene::rayTrace(const Ray& ray, const int depth = 0) const {
        glm::dvec3 rayColor { 0.0 };

        // Make sure we don't bounce forever
        if(depth >= 10)
            return rayColor;

        Ray::Intersection rayHit = intersect(ray);
        glm::dvec3 rayHitPosition { ray.origin + ray.direction * rayHit.distance };

        // Hit diffuse object
        if(rayHit.material.type == Material::Type::Diffuse) {
            for (Light* lightSource : lights) {
                PointLight* pointLight = dynamic_cast<PointLight*>(lightSource);
                AreaLight* areaLight = dynamic_cast<AreaLight*>(lightSource);

                if (pointLight) {
                    glm::dvec3 rayToLightSource = pointLight->origin - rayHitPosition;
                    glm::dvec3 rayToLightNormal { glm::normalize(rayToLightSource) };

                    Ray shadowRay { rayHitPosition + rayToLightNormal*Ray::EPSILON,
                            glm::normalize(rayToLightSource) };

                    Ray::Intersection shadowRayHit { intersect(shadowRay) };
                    if (shadowRayHit.distance >= glm::length(rayToLightSource)) {
                        double lambertianFalloff { std::max(0.0, glm::dot(shadowRay.direction, rayHit.normal)) };
                        rayColor += lightSource->color * rayHit.material.color * lambertianFalloff;
                    } 
                }
                else if (areaLight) {
                    double shadowRayCount{10};
                    double totalFalloff{};
                    std::vector<glm::dvec3> lightOrigins;
                    for (int i=0; i<shadowRayCount; i++) lightOrigins.push_back(areaLight->sample());

                    for (const glm::dvec3& origin : lightOrigins) {
                        glm::dvec3 rayToLightSource = origin - rayHitPosition;
                        glm::dvec3 rayToLightNormal { glm::normalize(rayToLightSource) };

                        Ray shadowRay { rayHitPosition + rayToLightNormal*Ray::EPSILON,
                                glm::normalize(rayToLightSource) };

                        Ray::Intersection shadowRayHit { intersect(shadowRay) };
                        if (shadowRayHit.distance >= glm::length(rayToLightSource)) {
                            double lambertianFalloff { std::max(0.0, glm::dot(shadowRay.direction, rayHit.normal)) };
                            
                            double cosa = glm::dot(shadowRay.direction, rayHit.normal);
                            double cosb = glm::dot(-shadowRay.direction, areaLight->normal);
                            if (cosb < 0.0) cosb = glm::dot(-shadowRay.direction, -areaLight->normal);

                            double geometricTerm = cosa*cosb/shadowRayHit.distance/shadowRayHit.distance;
                            totalFalloff += lambertianFalloff*geometricTerm;
                        }
                    }
                    rayColor +=  areaLight->area * lightSource->color * rayHit.material.color * totalFalloff / shadowRayCount;
                }
            }
        }
        // Hit specular, mirror-like surface.
        else if(rayHit.material.type == Material::Type::Reflective) {
            Ray reflectionRay { ray.reflect(rayHitPosition, rayHit.normal) };
            rayColor += rayTrace(reflectionRay, depth + 1) * 0.9; // Falloff.
        }
        // Hit specular, transparent surface.
        else if(rayHit.material.type == Material::Type::Refractive) {
            double kr = ray.fresnel(rayHit.normal, rayHit.material.refractionIndex);
            bool outside = glm::dot(ray.direction, rayHit.normal) < 0.0;
            glm::dvec3 refractionColor = glm::dvec3(0.0);
            glm::dvec3 refractionDir;

            if(kr < 1.0) { // Check if ray isn't completely parallel to graze.
                Ray refractionRay { ray.refract(rayHitPosition, rayHit.normal,
                                                rayHit.material.refractionIndex) };
                refractionColor = rayTrace(refractionRay,depth + 1);
            }

            Ray reflectionRay; // If we need to invert the bias if we are inside.
            if (outside) reflectionRay = ray.reflect(rayHitPosition, rayHit.normal);
            else reflectionRay = ray.insideReflect(rayHitPosition, rayHit.normal);
            glm::dvec3 reflectionColor = rayTrace(reflectionRay, depth + 1);
            rayColor += reflectionColor * kr + refractionColor * (1.0 - kr);
        }

        return rayColor;
    }
}
