#include "mcrt/photon_map.hh"

#include <algorithm>

void mcrt::PhotonMap::rebalance() {
    std::vector<const Photon*> photonPointers;
    photonPointers.reserve(photons.size());
    // Basically just populates the vector created above with pointers to photons.
    std::transform(photons.cbegin(), photons.cend(), std::back_inserter(photonPointers),
                  [](const Photon& photon) {
        return &photon;
    });

    // Start off by splitting the x-axis.
    root = new KdNode { KdNode::Axis::X };
    root->construct(photonPointers);
    rebalanced = true;
}

// We are going to sort in each step. Yea I know it's bloody expensive; but fuck it.
void mcrt::PhotonMap::KdNode::construct(const std::vector<const Photon*>& photons) {
}

// Finds out the median photon based on the other photons' position in the k-d splitting axis plane.
const mcrt::Photon* mcrt::PhotonMap::KdNode::median(const std::vector<const Photon*>& photons) const {
    if (photons.empty()) return nullptr;
    return photons[photons.size() >> 1];
}

// Sorts the photon map based on the current kd-node's splitting axis.
void mcrt::PhotonMap::KdNode::sort(std::vector<const Photon*>& photons) {
    std::sort(photons.begin(), photons.end(),
              [this](const Photon* a, const Photon* b) {
        return a->position[axis] < b->position[axis];
    });
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

void mcrt::PhotonMap::print(std::ostream& output, const std::vector<const Photon*>& otherPhotons) const {
    output << "x,y,z,vx,vy,vz,r,g,b\n";
    for (const auto& photon : photons) {
        output << photon.position.x << ',' << photon.position.y << ',' << photon.position.z << ',';
        output << photon.incoming.x << ',' << photon.incoming.y << ',' << photon.incoming.z << ',';

        bool photonFound { false };
        for (auto otherPhoton : otherPhotons) {
            if (&photon == otherPhoton) {
                photonFound = true;
                break;
            }
        }

        if (photonFound) output << 0.4 << ',' << 0.4 << ',' << 0.4 << std::endl;
        else             output << 0.8 << ',' << 0.8 << ',' << 0.8 << std::endl;
    }
}

std::vector<const mcrt::Photon*> mcrt::PhotonMap::neighbors(const glm::dvec3& around, size_t k) const {
    if (rebalanced == false) std::cerr << "Warning! Photon map hasn't been re-balanced!?" << std::endl;
    return {  };
}

std::ostream& operator<<(std::ostream& output, const mcrt::PhotonMap& photonMap) {
    photonMap.print(output);
    return output;
}
