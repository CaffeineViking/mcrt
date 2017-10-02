#ifndef MCRT_LIGHTS_HH
#define MCRT_LIGHTS_HH

#include <glm/glm.hpp>
#include <random>

#include "mcrt/ray.hh"
#include "mcrt/scene.hh"

namespace mcrt {
    class Scene;
    struct Light {
        enum Type {
            PointLight,
            AreaLight
        };
        Light(glm::dvec3, double);
        virtual ~Light() = 0;
        glm::dvec3 color;
        double intensity;
        virtual glm::dvec3 radiance(const Ray::Intersection&, const Scene*) = 0;
        virtual Ray::Intersection intersect(const Ray&) const = 0;
    };

    struct PointLight : public Light {
        PointLight(glm::dvec3, glm::dvec3, double);
        ~PointLight() {};
        glm::dvec3 origin;

        glm::dvec3 radiance(const Ray::Intersection&, const Scene*) override;
        Ray::Intersection intersect(const Ray&) const override;
   
    };

    struct AreaLight : public Light {
        AreaLight(glm::dvec3,glm::dvec3, glm::dvec3, glm::dvec3, double);
        ~AreaLight() {};
        glm::dvec3 v0;
        glm::dvec3 v1;
        glm::dvec3 v2;
        glm::dvec3 normal;
        double area;
        static size_t shadowRayCount;

        glm::dvec3 sample();
        glm::dvec3 radiance(const Ray::Intersection&, const Scene*) override;
        Ray::Intersection intersect(const Ray&) const override;        

    private:
        std::random_device rd;
    };
}

#endif
