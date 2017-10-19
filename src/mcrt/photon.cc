#include "mcrt/photon.hh"

#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& output, const mcrt::Photon& photon) {
    output << "Photon {\n"
           << "    position: " << glm::to_string(photon.position)
           << ",\n    incoming: " << glm::to_string(photon.incoming)
           << ",\n    color: " << glm::to_string(photon.color) << "\n}";
    return output;
}
