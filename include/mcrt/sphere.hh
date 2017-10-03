#ifndef MCRT_SPHERE_HH
#define MCRT_SPHERE_HH

#include "mcrt/ray.hh"
#include "mcrt/material.hh"
#include "mcrt/geometry.hh"

namespace mcrt {

    class Sphere : public Geometry {
    private:
        glm::dvec3 _origin;
        double _radius;
    public:
        Sphere(const glm::dvec3 o, double r, Material* m);
        Ray::Intersection intersect(const Ray& ray) const override;
    };

}
#endif
