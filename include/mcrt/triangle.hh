#ifndef MCRT_TRIANGLE_HH
#define MCRT_TRIANGLE_HH

#include "mcrt/ray.hh"
#include "mcrt/material.hh"
#include "mcrt/geometry.hh"

namespace mcrt {

    class Triangle : public Geometry{
    private:
        glm::dvec3 _v1;
        glm::dvec3 _v2;
        glm::dvec3 _v3;
    public:
        Triangle(const glm::dvec3& v1,const glm::dvec3& v2,const glm::dvec3& v3, Material* m);
        Ray::Intersection intersect(const Ray& ray) const override;
    };

}
#endif
