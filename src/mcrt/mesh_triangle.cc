#include "mcrt/mesh_triangle.hh"

#include <glm/gtx/projection.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

namespace mcrt {
    MeshTriangle::MeshTriangle(const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3,
                               const glm::dvec3& n1, const glm::dvec3& n2, const glm::dvec3& n3,
                               const Material& m) : Triangle { v1, v2, v3, m },
                                                    _n1{n1}, _n2{n2}, _n3{n3} {  }

    Ray::Intersection MeshTriangle::intersect(const Ray& ray) const {
        // Probably some sort of normal interpolation should go here.
        return Triangle::intersect(ray);
    }

    void MeshTriangle::move(const glm::dvec3& p) {
        _v1 += p;
        _v2 += p;
        _v3 += p;
    }

    void MeshTriangle::scale(const double c) {
        _v1 *= c;
        _v2 *= c;
        _v3 *= c;
    }

    void MeshTriangle::rotate(const glm::dvec3& axis, double radius) {
        _v1 = glm::rotate(_v1, radius, axis);
        _v2 = glm::rotate(_v2, radius, axis);
        _v3 = glm::rotate(_v3, radius, axis);
    }

    glm::dmat3 MeshTriangle::getCorners() {
        return {_v1, _v2, _v3};
    }

    std::ostream& MeshTriangle::write(std::ostream& stream) const {
        stream << "Triangle:\n"
            << glm::to_string(_v1) << "\n"
            << glm::to_string(_v2) << "\n"
            << glm::to_string(_v3) << "\n";
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, MeshTriangle const& t) {
        return t.write(stream);
    }
}
