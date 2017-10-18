#ifndef MCRT_PHOTON_MAP_HH
#define MCRT_PHOTON_MAP_HH

#include <vector>

#include "mcrt/photon.hh"
#include "mcrt/bounding_sphere.hh"

namespace mcrt {
    class PhotonMap {
    public:
        PhotonMap() = default;
        PhotonMap(const std::vector<Photon>& photons)
            : photons { photons } {  }

        void rebalance();
        void insert(const Photon&);
        void remove(const Photon&);

        std::vector<Photon*> nearestNeighbors(size_t) const;
        std::vector<Photon*> inside(const BoundingSphere&) const;

    private:
        std::vector<Photon> photons;
    };
}

#endif
