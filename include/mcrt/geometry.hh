#ifndef MCRT_GEOMETRY_HH
#define MCRT_GEOMETRY_HH

#include "mcrt.hh"

namespace mcrt {
    
    class Geometry {
    protected:
	Material _material;       
	Geometry(const Material& m);

    public:
	virtual Intersection intersect(const Ray& ray) const = 0;

	Material getMaterial() const;
    };

}
#endif
