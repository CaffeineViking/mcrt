#include "mcrt/triangle.hh"

namespace mcrt {
    Triangle::Triangle(const glm::dvec3& v1,const glm::dvec3& v2,const glm::dvec3& v3, const Material& m):
                       Geometry(m), _v1{v1}, _v2{v2}, _v3{v3} { }

    // Returns distance from ray to triangle, 0 means no intersection.
    Ray::Intersection Triangle::intersect(const Ray& ray) const {
        glm::dvec3 e1 = _v2 - _v1;
        glm::dvec3 e2 = _v3 - _v1;

        // Surface normal I think
        glm::dvec3 pvec = glm::cross(ray.direction,e2);
        double det = glm::dot(e1,pvec);

        glm::dvec3 normal { glm::normalize(glm::cross(e1, e2)) };
        if (glm::dot(normal, ray.direction) > 0) normal = -normal;

        Ray::Intersection result {0, normal, {glm::dvec3(0.0), Material::Type::Diffuse, 0.0},glm::dvec3(0)};

        if(det < Ray::EPSILON && det > -Ray::EPSILON) {
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
        result.position = ray.origin + ray.direction * result.distance;
        return result;
    }
}
