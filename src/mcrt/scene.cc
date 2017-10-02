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

        return closestHit;
    }

    // Will be our resource after this...
    void Scene::add(Geometry* geometry) {
        geometries.push_back(geometry);
    }

    void Scene::add(const PointLight& light) {
        lights.push_back(light);
    }

    size_t Scene::maxRayDepth = 10;

    glm::dvec3 Scene::rayTrace(const Ray& ray, const size_t depth = 0) const {
        glm::dvec3 rayColor { 0.0 };

        // Make sure we don't bounce forever
        if(depth >= Scene::maxRayDepth)
            return rayColor;

        Ray::Intersection rayHit = intersect(ray);
        glm::dvec3 rayHitPosition { ray.origin + ray.direction * rayHit.distance };

        // Hit diffuse object
        if(rayHit.material.type == Material::Type::Diffuse) {
            for (const PointLight& lightSource : lights) {

                glm::dvec3 rayToLightSource = lightSource.origin - rayHitPosition;
                glm::dvec3 rayToLightNormal { glm::normalize(rayToLightSource) };

                Ray shadowRay { rayHitPosition + rayToLightNormal*Ray::EPSILON,
                                glm::normalize(rayToLightSource) };

                Ray::Intersection shadowRayHit { intersect(shadowRay) };
                if (shadowRayHit.distance >= glm::length(rayToLightSource)) {
                    double lambertianFalloff { std::max(0.0, glm::dot(shadowRay.direction, rayHit.normal)) };
                    rayColor += lightSource.color * rayHit.material.color * lambertianFalloff;
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
