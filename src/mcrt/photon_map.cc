#include "mcrt/photon_map.hh"

void mcrt::PhotonMap::rebalance() {
    rebalanced = true;
}

void mcrt::PhotonMap::insert(const Photon& photon) {
    photons.push_back(photon);
    rebalanced = false;
}

void mcrt::PhotonMap::remove(size_t index) {
    photons.erase(photons.begin() + index);
    rebalanced = false;
}

void mcrt::PhotonMap::print(std::ostream& output) const {
    output << "x,y,z,vx,vy,vz,r,g,b\n";
    for (const auto& photon : photons) {
        output << photon.position.x << ',' << photon.position.y << ',' << photon.position.z << ',';
        output << photon.incoming.x << ',' << photon.incoming.y << ',' << photon.incoming.z << ',';
        output << photon.color.r << ',' << photon.color.g << ',' << photon.color.b << std::endl;
    }
}

void mcrt::PhotonMap::print(std::ostream& output, const std::vector<Photon*>& photons) const {
}

std::vector<mcrt::Photon*> mcrt::PhotonMap::neighbors(const glm::dvec3& around, size_t k) {
    if (rebalanced == false)
        rebalance();
}

std::ostream& operator<<(std::ostream& output, const mcrt::PhotonMap& photonMap) {
    photonMap.print(output);
    return output;
}
