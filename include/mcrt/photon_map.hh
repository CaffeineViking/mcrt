#ifndef MCRT_PHOTON_MAP_HH
#define MCRT_PHOTON_MAP_HH

#include <vector>
#include <iostream>

#include "mcrt/photon.hh"

namespace mcrt {
    class PhotonMap final {
    public:
        PhotonMap() = default;
        PhotonMap(const std::vector<Photon>& photons)
            : photons { photons } { rebalance(); }

        ~PhotonMap() {
            if (root != nullptr)
                delete root;
            root = nullptr;
        }

        PhotonMap(PhotonMap&& photonMap) {
            rebalanced = photonMap.rebalanced;
            photons = photonMap.photons;
            root = photonMap.root;
            photonMap.root = nullptr;
        }

        PhotonMap& operator=(PhotonMap&& photonMap) {
            rebalanced = photonMap.rebalanced;
            photons = photonMap.photons;
            root = photonMap.root;
            photonMap.root = nullptr;
            return *this;
        }

        void rebalance();

        void insert(const Photon&);
        void remove(size_t pindex);

        void print(std::ostream&) const;
        void print(std::ostream&, const std::vector<Photon*>&) const;

        bool isBalanced() const { return rebalanced; }
        std::vector<Photon*> neighbors(const glm::dvec3&, size_t);

    private:
        struct KdNode {
            enum Axis {
                X = 0,
                Y = 1,
                Z = 2
            } axis;

            ~KdNode() {
                if (left != nullptr)
                    delete left;
                if (right != nullptr)
                    delete right;

                right = nullptr;
                left = nullptr;

                photon = nullptr;
            }

            KdNode *left   { nullptr },
                   *right  { nullptr };
            Photon* photon { nullptr };
        };

        KdNode* root { nullptr };
        bool rebalanced { false };

        std::vector<Photon> photons;
    };
}

std::ostream& operator<<(std::ostream&, const mcrt::PhotonMap&);

#endif
