#ifndef MCRT_GEOMETRY_HH
#define MCRT_GEOMETRY_HH

#include "mcrt/ray.hh"
#include "mcrt/material.hh"

namespace mcrt {

    class Geometry {
    protected:
        Material* _material;
        Geometry(Material* m);

    public:
        virtual ~Geometry() = default;
        virtual Ray::Intersection intersect(const Ray& ray) const = 0;

        Material* getMaterial() const;
    };

}
#endif
