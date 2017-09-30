#ifndef MCRT_BOUNDING_SPHERE
#define MCRT_BOUNDING_SPHERE

#include <glm/glm.hpp>
#include "mcrt/ray.hh"

namespace mcrt {

    class BoundingSphere {
    public:
        BoundingSphere() = default;
        BoundingSphere(const glm::dvec3&, const double&);
        Ray::Intersection intersect(const Ray&) const;

    private:
        glm::dvec3 _origin;
        double _radius;
    };
}
#endif
