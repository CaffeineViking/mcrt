#ifndef MCRT_MESH_TRIANGLE_HH
#define MCRT_MESH_TRIANGLE_HH

#include <iostream>
#include "mcrt/ray.hh"
#include "mcrt/triangle.hh"
#include "mcrt/geometry.hh"

namespace mcrt {
    class MeshTriangle : public Triangle {
    private:
        glm::dvec3 _n1;
        glm::dvec3 _n2;
        glm::dvec3 _n3;

    public:
        MeshTriangle(const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3,
                     const glm::dvec3& n1, const glm::dvec3& n2, const glm::dvec3& n3,
                     Material* m);

        Ray::Intersection intersect(const Ray& ray) const override;
        void rotate(const glm::dvec3&, double radius);
        void move(const glm::dvec3&);
        void scale(const double);

        std::ostream& write(std::ostream& stream) const;
        glm::dmat3 getCorners();
    };

    std::ostream& operator<<(std::ostream& stream, MeshTriangle const& t);
}
#endif
