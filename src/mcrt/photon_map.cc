#include "mcrt/photon_map.hh"

#include <algorithm>

#include "mcrt/progress.hh"

void mcrt::PhotonMap::rebalance() {
    std::vector<const Photon*> photonPointers;
    photonPointers.reserve(photons.size());

    // Basically just populates the vector created above with pointers to photons.
    std::transform(photons.cbegin(), photons.cend(), std::back_inserter(photonPointers),
                  [](const Photon& photon) {
        return &photon;
    });

    if (root != nullptr)
        delete root;

    KdNode::processed = 0;
    KdNode::cachedProgress = 0.0;
    KdNode::total = photons.size();

    // Start off by splitting the x-axis.
    root = new KdNode { KdNode::Axis::X };
    root->construct(photonPointers);
    printProgress("Balance k-d: ", 1.0);
    std::cout << std::endl;
    rebalanced = true;
}

std::size_t mcrt::PhotonMap::KdNode::total;
std::size_t mcrt::PhotonMap::KdNode::processed;
double mcrt::PhotonMap::KdNode::cachedProgress;

// We are going to sort in each step. Yes it's bloody expensive but fuck it :)
void mcrt::PhotonMap::KdNode::construct(std::vector<const Photon*>& photons) {
    if (photons.size() == 1) {
        photon = photons[0];
        return;
    }

    sort(photons); // Sort along an split axis.
    std::size_t medianPhoton = median(photons);
    photon = photons[medianPhoton];

    processed += 2;
    double progress { getProgress() };
    if (progress - cachedProgress >= 0.01) {
        if (progress < 1.0)
            printProgress("Balance k-d: ",
                          progress);
        cachedProgress = progress;
    }

    std::size_t leftSize { medianPhoton };
    std::size_t rightSize { photons.size() - leftSize  - 1 };

    if (leftSize != 0) {
        std::vector<const Photon*> leftPhotons(leftSize);
        std::copy(photons.cbegin(), photons.cbegin() + medianPhoton, leftPhotons.begin());
        left = new KdNode { getNextAxis() };
        left->construct(leftPhotons);
    }

    if (rightSize != 0) {
        std::vector<const Photon*> rightPhotons(rightSize);
        std::copy(photons.cbegin() + medianPhoton + 1, photons.cend(), rightPhotons.begin());
        right = new KdNode { getNextAxis() };
        right->construct(rightPhotons);
    }
}

// Finds out the median photon based on the other photon position in the same split axis.
size_t mcrt::PhotonMap::KdNode::median(const std::vector<const Photon*>& photons) const {
    if (photons.empty()) return photons.size();
    return photons.size() >> 1;
}

// Sorts the photon map based on the current kd-node's splitting axis.
void mcrt::PhotonMap::KdNode::sort(std::vector<const Photon*>& photons) {
    std::sort(photons.begin(), photons.end(),
              [this](const Photon* a, const Photon* b) {
        return a->position[axis] < b->position[axis];
    });
}

// Since k-d cycle (everywhere I've seen anyway) the split in a modular way.
mcrt::PhotonMap::KdNode::Axis mcrt::PhotonMap::KdNode::getNextAxis() const {
    return static_cast<Axis>((static_cast<char>(axis) + 1) % 3);
}

// Beam into the k-d node regions trying to find photons which are inside a given sphere.
void mcrt::PhotonMap::KdNode::around(const glm::dvec3& sphereOrigin, double sphereRadius,
                                     std::vector<const Photon*>& photonsInSphere) const {
    double axisRadius { photon->position[axis] - sphereOrigin[axis] };
    if (axisRadius * axisRadius <= sphereRadius * sphereRadius) {
        if (left  != nullptr) left->around(sphereOrigin,  sphereRadius, photonsInSphere);
        if (right != nullptr) right->around(sphereOrigin, sphereRadius, photonsInSphere);
        if (glm::distance(sphereOrigin, photon->position) <= sphereRadius) {
            photonsInSphere.push_back(photon);
        }
    } else if (left != nullptr  && photon->position[axis] >  sphereOrigin[axis]) {
        left->around(sphereOrigin, sphereRadius, photonsInSphere);
    } else if (right != nullptr && photon->position[axis] <= sphereOrigin[axis]) {
        right->around(sphereOrigin, sphereRadius, photonsInSphere);
    }
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
        if (photon.shadow) output << 1.0 << ',' << 0.0 << ',' << 0.0 << std::endl;
        else output << photon.color.r << ',' << photon.color.g << ',' << photon.color.b << std::endl;
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

        if (photonFound) output << 0.0 << ',' << 0.0 << ',' << 0.0 << std::endl;
        else             output << 0.6 << ',' << 0.6 << ',' << 0.6 << std::endl;
    }
}

std::vector<const mcrt::Photon*> mcrt::PhotonMap::around(const glm::dvec3& origin, double radius) const {
    if (rebalanced == false) std::cerr << "Stop! A photon map hasn't been re-balanced yet!" << std::endl;
    std::vector<const Photon*> photons;
    root->around(origin, radius, photons);
    return photons; // around out spheres.
}

std::ostream& operator<<(std::ostream& output, const mcrt::PhotonMap& photonMap) {
    photonMap.print(output);
    return output;
}
