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
        void print(std::ostream&, const std::vector<const Photon*>&) const;

        bool isBalanced() const { return rebalanced; }
        std::vector<const Photon*> neighbors(const glm::dvec3&, size_t) const;

    private:
        struct KdNode {
            enum Axis : char {
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

            // Recursively builds the kd-tree until it's done.
            void construct(const std::vector<const Photon*>&);
            // Helper functions for finding out the median by sorting it.
            const Photon* median(const std::vector<const Photon*>&) const;
            void sort(std::vector<const Photon*>&);

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
