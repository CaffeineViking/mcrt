#include "mcrt/photon_map.hh"

void mcrt::PhotonMap::rebalance() {
}

void mcrt::PhotonMap::insert(const Photon& photon) {
    photons.push_back(photon);
}

void mcrt::PhotonMap::remove(size_t index) {
    photons.erase(photons.begin() + index);
}

std::vector<mcrt::Photon*> mcrt::PhotonMap::insideSphere(const BoundingSphere& sphere) const {
}

std::vector<mcrt::Photon*> mcrt::PhotonMap::nearestNeighbors(const glm::dvec3& around, size_t amount) const {
}
