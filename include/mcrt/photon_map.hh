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
        void remove(size_t pindex);

        std::vector<Photon*> insideSphere(const BoundingSphere&) const;
        std::vector<Photon*> nearestNeighbors(const glm::dvec3&, size_t) const;

    private:
        struct KdNode {
            enum Axis {
                X = 0,
                Y = 1,
                Z = 2
            } axis;

            double axisSplit;

            KdNode *left   { nullptr },
                   *right  { nullptr };

            Photon* photon { nullptr };
        };

        std::vector<Photon> photons;
    };
}

#endif
