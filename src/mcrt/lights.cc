#include "mcrt/lights.hh"
#include <random>

namespace mcrt {
    Light::Light(glm::dvec3 color) : color(color) {};
    Light::~Light() {}

    PointLight::PointLight(glm::dvec3 origin, glm::dvec3 color) : Light(color), origin(origin) {}
 
    AreaLight::AreaLight(glm::dvec3 v0, glm::dvec3 v1, glm::dvec3 v2, glm::dvec3 color) : Light(color), v0(v0), v1(v1), v2(v2) 
    {
        normal = glm::normalize(glm::cross(v1-v0, v2-v0));
        area = 0.5*glm::length(glm::cross(v1-v0, v2-v0));
    }
 
    glm::dvec3 AreaLight::sample() {     
        double u;
        double v;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0,1);

        do {
            u = dist(gen);
            v = dist(gen);
        } while (u + v >= 1);

        return (1 - u - v)*v0 + u*v1 + v*v2;
    }

    Ray::Intersection AreaLight::intersect(const Ray& ray) const {
        glm::dvec3 e1 = v1 - v0;
        glm::dvec3 e2 = v2 - v0;

        // Surface normal I think
        glm::dvec3 pvec = glm::cross(ray.direction,e2);
        double det = glm::dot(e1,pvec);

        glm::dvec3 normal { glm::normalize(glm::cross(e1, e2)) };
        if (glm::dot(normal, ray.direction) > 0) normal = -normal;

        Ray::Intersection result {0, normal, {glm::dvec3(0.0), Material::Type::Diffuse, 0.0}};

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
        return result;
    }
}
