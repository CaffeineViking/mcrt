#include "mcrt/ray.hh"

#include <glm/gtx/rotate_vector.hpp> 
#include <glm/gtc/constants.hpp>

glm::dvec3 mcrt::Ray::Intersection::sampleHemisphere(const Ray& i) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);

    // Uses the cosine-weighted sampling over the
    // hemisphere for filter out unimportant rays.
    double phi = dis(gen) * glm::pi<double>() * 2.0 / material->reflectionRate;
    if (phi > 2.0*glm::pi<double>()) return glm::dvec3{};
    double theta = std::asin(std::sqrt(dis(gen)));

    glm::dvec3 v1 = glm::normalize(i.direction - glm::dot(i.direction, normal) * normal);
    glm::dvec3 v2 = glm::normalize(glm::cross(v1,normal));

    const glm::dvec3 azimuthRotation = glm::rotate(v1, phi, normal);
    const glm::dvec3 inclinationRotation = glm::rotate(v1, theta, v2);
    const glm::dvec3 outgoing = glm::normalize(azimuthRotation + inclinationRotation);
    return outgoing;
}
