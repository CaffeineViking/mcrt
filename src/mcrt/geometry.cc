#include "mcrt/geometry.hh"

namespace mcrt {
    Geometry::Geometry(const Material& m): 
	_material{m}
    {}

    Material Geometry::getMaterial() const {
        return _material;
    }
}
