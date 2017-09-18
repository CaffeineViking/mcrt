#include "mcrt/triangle.hh"
#include <glm/gtx/string_cast.hpp>

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

        Ray::Intersection result {0, normal, {glm::dvec3(0.0), Material::Type::Diffuse, 0.0}};

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
        return result;
    }

    void Triangle::move(glm::dvec3 p) {
	_v1 += p;
	_v2 += p;
	_v3 += p;
    }

    void Triangle::scale(const double& c) {
	_v1 *= c;
	_v2 *= c;
	_v3 *= c;
    }

    std::ostream& Triangle::write(std::ostream& stream) const {
	stream << "Triangle:\n"
	       << glm::to_string(_v1) << "\n"
	       << glm::to_string(_v2) << "\n"
	       << glm::to_string(_v3) << "\n";
	return stream;
    }

    std::ostream& operator<<(std::ostream& stream, Triangle const& t) {
	return t.write(stream);
    }
}
