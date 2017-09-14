#ifndef MCRT_SPHERE_HH
#define MCRT_SPHERE_HH

#include "mcrt.hh"
#include "geometry.hh"

namespace mcrt {

    class Sphere : public Geometry {
    private:
	glm::dvec3 _origin;          
	double _radius;
    public:
	Sphere(const glm::dvec3 o, double r, const Material& m);
	Intersection intersect(const Ray& ray) const override;
    };

}
#endif
