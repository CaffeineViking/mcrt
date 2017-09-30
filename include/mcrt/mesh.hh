#ifndef MCRT_MESH
#define MCRT_MESH

#include <vector>
#include <glm/glm.hpp>

#include "mcrt/ray.hh"
#include "mcrt/geometry.hh"
#include "mcrt/sphere.hh"
#include "mcrt/mesh_triangle.hh"
#include "mcrt/bounding_sphere.hh"

namespace mcrt {
    class Mesh : public Geometry {
    public:
        Mesh();
        ~Mesh() { for (auto t : _triangles) delete t; }
        Mesh(const Material&);

        void move(glm::dvec3);
        void scale(const double&);
        void rotateX(const double&);
        void rotateY(const double&);
        void rotateZ(const double&);

        void updateBoundingSphere();

        void setMaterial(const Material&);
        void addTriangle(MeshTriangle*);
        void addTriangle(const glm::dvec3&, const glm::dvec3&, const glm::dvec3&,
                         const glm::dvec3&, const glm::dvec3&, const glm::dvec3&);
        std::vector<MeshTriangle*> getTriangles() const;

        Ray::Intersection intersect(const Ray&) const override;

        void print();

    private:
        std::vector<MeshTriangle*> _triangles;
        BoundingSphere _bound;
    };
}

#endif
