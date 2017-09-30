#include "mcrt/geometry.hh"

namespace mcrt {
    Geometry::Geometry(Material* m)
        : material { m } {  }

    Material* Geometry::getMaterial() const {
        return material;
    }
}
