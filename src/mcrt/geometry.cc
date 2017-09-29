#include "mcrt/geometry.hh"

namespace mcrt {
    Geometry::Geometry(Material* m)
        : _material{m} {  }

    Material* Geometry::getMaterial() const {
        return _material;
    }
}
