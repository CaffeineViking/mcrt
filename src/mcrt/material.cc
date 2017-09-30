#include "mcrt/material.hh"

#include <algorithm>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>

glm::dvec3 mcrt::LambertianMaterial::brdf(const glm::dvec3&, const glm::dvec3&,
                                          const glm::dvec3&, const glm::dvec3&) const {
    return albedo / glm::pi<double>();
}

glm::dvec3 mcrt::OrenNayarMaterial::brdf(const glm::dvec3&, const glm::dvec3& normal,
                                         const glm::dvec3& incoming, const glm::dvec3& outgoing) const {
    double t { 1.0 }; // Derived in Yasuhiro Fujii's improved/simplified Oren-Nayar reflectance model.
    double s { glm::dot(outgoing, incoming) - glm::dot(normal, outgoing)*glm::dot(normal, incoming) };
    if (s > 0.0) t = std::max(glm::dot(normal, outgoing), glm::dot(normal, incoming));
    double roughnessFalloff { glm::pi<double>() + roughness*(glm::half_pi<double>() - (2.0/3.0)) };
    double A { 1.0 / roughnessFalloff }, B { roughness / roughnessFalloff };
    return albedo * (A + B*(s / t));
}
