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
        std::vector<const Photon*> around(const glm::dvec3&, double) const;

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

            // Recursively builds a kd-tree until done.
            void construct(std::vector<const Photon*>&);

            // Finds the photon's median index position by an axis.
            size_t median(const std::vector<const Photon*>&) const;
            void sort(std::vector<const Photon*>&);
            Axis getNextAxis() const;

            // Add all possible points inside a sphere to the given vector of photons.
            void around(const glm::dvec3&, double, std::vector<const Photon*>&) const;

            static std::size_t total;
            static std::size_t processed;
            static double cachedProgress;
            static double getProgress() {
                return processed / (double) total;
            }

            KdNode *left         { nullptr },
                   *right        { nullptr };
            const Photon* photon { nullptr };
        };

        KdNode* root { nullptr };
        bool rebalanced { false };

        std::vector<Photon> photons;
    };
}

std::ostream& operator<<(std::ostream&, const mcrt::PhotonMap&);

#endif
