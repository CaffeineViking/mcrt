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
            nullptr,
            glm::dvec3(0.0)
        };

        for (const Geometry* geometry : geometries) {
            Ray::Intersection rayHit = geometry->intersect(ray);
            if (rayHit.distance > 0.0 && rayHit.distance < closestHit.distance)
                closestHit = rayHit;
        }

        for (Light* lightSource : lights) {
            Ray::Intersection lightHit = lightSource->intersect(ray);
            if(lightHit.distance > 0.0 && lightHit.distance < closestHit.distance ){
                closestHit = lightHit;
            }
        }
        return closestHit;
    }

    double Scene::inShadow(const Ray& ray) const{

        double distance = std::numeric_limits<double>::max();

        for(const Geometry* geometry: geometries){
            Ray::Intersection rayHit = geometry->intersect(ray);

            if(rayHit.distance <= 0.0) {
                continue;
            }

            // Intersects Refractive surface
            if(rayHit.material->type == Material::Type::Refractive){
                continue;
            }
            distance = std::min(distance, rayHit.distance);
        }

        // Return distance to occlusion
        return distance;
    }

    // Will be our resource after this...
    void Scene::add(Geometry* geometry) {
        geometries.push_back(geometry);
    }

    void Scene::add(Material* material) {
        materials.push_back(material);
    }

    void Scene::add(Light* light) {
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

        // We have hit nothing or something like that I guess.....
        if (rayHit.material == nullptr) return glm::dvec3 { 0.0 };

        if(rayHit.material->type == Material::Type::Diffuse) {
            glm::dvec3 reflectionDir = rayHit.sampleHemisphere(ray);
            if (glm::length(reflectionDir) > 0.0) {
                Ray reflectionRay { rayHit.position + reflectionDir*Ray::EPSILON, reflectionDir };
                glm::dvec3 brdf = rayHit.material->brdf(rayHit.position, rayHit.normal, reflectionDir, -ray.direction);
                rayColor += rayTrace(ray, depth + 1) * brdf * glm::pi<double>() / rayHit.material->reflectionRate;
            }
            for (Light* lightSource : lights) {
                rayColor += lightSource->radiance(ray, rayHit, this);
            }

        } else if(rayHit.material->type == Material::Type::Reflective) {

            Ray reflectionRay { ray.reflect(rayHitPosition, rayHit.normal) };
            rayColor += rayTrace(reflectionRay, depth + 1) * 0.9; // Falloff.

        } else if(rayHit.material->type == Material::Type::Refractive) {

            double kr = ray.fresnel(rayHit.normal, rayHit.material->refractionIndex);
            bool outside = glm::dot(ray.direction, rayHit.normal) < 0.0;
            glm::dvec3 refractionColor = glm::dvec3(0.0);
            glm::dvec3 refractionDir;

            if(kr < 1.0) { // Check if ray isn't completely parallel to graze.
                Ray refractionRay { ray.refract(rayHitPosition, rayHit.normal,
                                                rayHit.material->refractionIndex) };
                refractionColor = rayTrace(refractionRay,depth + 1);
            }

            Ray reflectionRay; // If we need to invert the bias if we are inside.
            if (outside) reflectionRay = ray.reflect(rayHitPosition, rayHit.normal);
            else reflectionRay = ray.insideReflect(rayHitPosition, rayHit.normal);
            glm::dvec3 reflectionColor = rayTrace(reflectionRay, depth + 1);
            rayColor += reflectionColor * kr + refractionColor * (1.0 - kr);

        } else if(rayHit.material->type == Material::Type::LightSource) {
            rayColor = rayHit.material->color;
        }

        return rayColor;
    }
}
