#include "mcrt/lights.hh"
#include <iostream>
#include <glm/gtx/rotate_vector.hpp> 
#include <glm/gtc/constants.hpp>

namespace mcrt {
    // Need to allocate material because we use the
    // intersection ray structure for passing color.
    Light::Light(glm::dvec3 color, double intensity)
        : material{new LambertianMaterial{Material::Type::LightSource, color, 0.0, 0.0}},
          intensity{intensity} {};

    Light::~Light() {
        delete material;
    }

    PointLight::PointLight(glm::dvec3 origin, glm::dvec3 color, double intensity) : Light(color,intensity), origin(origin) {}
 
    glm::dvec3 PointLight::radiance(const Ray& ray, const Ray::Intersection& rayHit, const Scene* scene) {
        glm::dvec3 rayToLightSource = origin - rayHit.position;
        glm::dvec3 rayToLightNormal { glm::normalize(rayToLightSource) };

        Ray shadowRay { rayHit.position + rayToLightNormal*Ray::EPSILON,
                glm::normalize(rayToLightSource) };

        double oclusionDistance = scene->inShadow(shadowRay);
        if (oclusionDistance > glm::length(rayToLightSource)) {
            double lambertianFalloff { std::max(0.0, glm::dot(shadowRay.direction, rayHit.normal)) };
            glm::dvec3 brdf { rayHit.material->brdf(rayHit.position, rayHit.normal,
                                                    -ray.direction, shadowRay.direction) };
            return material->color * brdf * lambertianFalloff;
        }

        return glm::dvec3(0);
    }

    Ray::Intersection PointLight::intersect(const Ray&) const {
        Ray::Intersection result {0.0, glm::dvec3(0.0), material, glm::dvec3(0.0)};
       return result;
    }

    size_t AreaLight::shadowRayCount = 10;

    AreaLight::AreaLight(glm::dvec3 v0, glm::dvec3 v1, glm::dvec3 v2, glm::dvec3 color, double intensity) : Light(color,intensity), v0(v0), v1(v1), v2(v2) 
    {
        normal = glm::normalize(glm::cross(v1-v0, v2-v0));
        area = 0.5*glm::length(glm::cross(v1-v0, v2-v0));
    }
     
    glm::dvec3 AreaLight::sampleHemisphere() const {
       static std::random_device rd; 
       static std::mt19937 gen(rd());
       static std::uniform_real_distribution<double> dis(0.0, 1.0);


       // Uses the cosine-weighted sampling over the
       // hemisphere for filter out unimportant rays.
       double phi = dis(gen) * glm::pi<double>() * 2.0;
       double theta = std::asin(std::sqrt(dis(gen)));

       glm::dvec3 v1 = glm::normalize(sample() - sample());
       glm::dvec3 v2 = glm::normalize(glm::cross(v1,normal));
       
       const glm::dvec3 azimuthRotation = glm::rotate(v1, phi, normal);
       const glm::dvec3 inclinationRotation = glm::rotate(normal, theta, v2);
       const glm::dvec3 outgoing = glm::normalize(azimuthRotation + inclinationRotation);
       return outgoing;
    }

    glm::dvec3 AreaLight::sample() const{     
        double u;
        double v;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dist(0,1);

        do {
            u = dist(gen);
            v = dist(gen);
        } while (u + v >= 1);

        return (1 - u - v)*v0 + u*v1 + v*v2;
    }

    Ray::Intersection AreaLight::intersect(const Ray& ray) const {
        glm::dvec3 e1 = v1 - v0;
        glm::dvec3 e2 = v2 - v0;

        glm::dvec3 pvec = glm::cross(ray.direction,e2);
        double det = glm::dot(e1,pvec);

        glm::dvec3 normal { glm::normalize(glm::cross(e1, e2)) };
        if (glm::dot(normal, ray.direction) > 0) normal = -normal;

        Ray::Intersection result {0.0, normal, material, glm::dvec3(0.0)};

        if(det < Ray::EPSILON && det > -Ray::EPSILON) {
            return result;
        }

        double inv_det = 1.0 / det;
        glm::dvec3 tvec = ray.origin - v0;
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
        result.position = ray.origin + ray.direction*result.distance;
        return result;
    }

    glm::dvec3 AreaLight::radiance(const Ray& ray, const Ray::Intersection& rayHit, const Scene* scene) {
        glm::dvec3 radiance(0.0);
        std::vector<glm::dvec3> lightOrigins;
        for (size_t i=0; i<shadowRayCount; i++) lightOrigins.push_back(sample());

        for (const glm::dvec3& origin : lightOrigins) {
            glm::dvec3 rayToLightSource = origin - rayHit.position;
            glm::dvec3 rayToLightNormal { glm::normalize(rayToLightSource) };

            Ray shadowRay { rayHit.position + rayToLightNormal * Ray::EPSILON,
                    rayToLightNormal };


            // Return distance to light, 0 if occluded
            double occlusionDistance = scene->inShadow(shadowRay);
            double shadowRayDistance = std::max(glm::distance(rayHit.position, origin),1.0);
            if (occlusionDistance > 0.0 && occlusionDistance >= shadowRayDistance) {
                double cosa = glm::dot(shadowRay.direction, rayHit.normal);
                double cosb = std::max(glm::dot(-shadowRay.direction, normal), glm::dot(-shadowRay.direction, -normal)) ;
                glm::dvec3 brdf = rayHit.material->brdf(rayHit.position, rayHit.normal,
                                                        -ray.direction, shadowRay.direction);
                radiance += brdf*cosa*cosb/(shadowRayDistance*shadowRayDistance);
            }
        }
        return area * material->color * radiance * intensity / (double)shadowRayCount;
    }
}
