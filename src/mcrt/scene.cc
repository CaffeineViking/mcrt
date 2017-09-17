#include "mcrt/scene.hh"
#include <glm/glm.hpp>
#include <limits>
#include <vector>
#include <cmath>

namespace mcrt {
    Intersection Scene::intersect(const Ray& ray) const {
        Intersection closestHit {
            std::numeric_limits<double>::max(),
            glm::dvec3(0.0), glm::dvec3(0.0)
        };

        for (const Geometry* geometry : geometries) {
            Intersection rayHit = geometry->intersect(ray);
            if (rayHit.distance > 0.0 && rayHit.distance < closestHit.distance)
                closestHit = rayHit;
        }

        return closestHit;
    }

    void Scene::add(const Geometry& geometry) {
        geometries.push_back(&geometry);
    }

    void Scene::add(const Light& light) {
        lights.push_back(light);
    }

    glm::dvec3 Scene::rayTrace(const Ray& ray) const {
        glm::dvec3 rayColor { 0.0 };
        Intersection rayHit = intersect(ray);

        for (const Light& lightSource : lights) {
            glm::dvec3 rayHitPosition { ray.origin + ray.direction * rayHit.distance };

            glm::dvec3 rayToLightSource = lightSource.origin - rayHitPosition;
            glm::dvec3 rayToLightNormal { glm::normalize(rayToLightSource) };

            Ray shadowRay { rayHitPosition + rayToLightNormal*1e-8,
                            glm::normalize(rayToLightSource) };

            Intersection shadowRayHit { intersect(shadowRay) };
            if (shadowRayHit.distance >= glm::length(rayToLightSource)) {
                double lambertianFalloff { std::max(0.0, glm::dot(shadowRay.direction, rayHit.normal)) };
                rayColor = lightSource.color * rayHit.material.color * lambertianFalloff;
            }
        }

        return rayColor;
    }
}
