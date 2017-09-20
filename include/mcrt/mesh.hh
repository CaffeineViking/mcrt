#ifndef MCRT_MESH
#define MCRT_MESH

#include "geometry.hh"
#include "triangle.hh"
#include "sphere.hh"
#include "mcrt.hh"
#include "bounding_sphere.hh"
#include <vector>
#include <glm/glm.hpp>

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
        void addTriangle(Triangle*);
        void addTriangle(glm::dvec3, glm::dvec3, glm::dvec3);
        std::vector<Triangle*> getTriangles() const;

        Intersection intersect(const Ray&) const override;

        void print();
	
    private:
        std::vector<Triangle*> _triangles;
        BoundingSphere _bound;
    };
}

#endif
