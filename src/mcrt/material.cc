#include "mcrt/material.hh"

#include <algorithm>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>

glm::dvec3 mcrt::LambertianMaterial::brdf(const glm::dvec3& normal, const glm::dvec3& incomingDirection,
                                                                    const glm::dvec3& outgoingDirection) const {
    return albedo / glm::pi<double>();
}

glm::dvec3 mcrt::OrenNayarMaterial::brdf(const glm::dvec3& normal, const glm::dvec3& incomingDirection,
                                                                   const glm::dvec3& outgoingDirection) const {
    glm::dvec3 incomingProjected { incomingDirection - glm::proj(incomingDirection, normal) },
               outgoingProjected { outgoingProjected - glm::proj(outgoingDirection, normal) };

    double incomingAzimuth { glm::orientedAngle(incomingProjected, outgoingProjected, incomingProjected) },
           outgoingAzimuth { glm::orientedAngle(incomingProjected, outgoingProjected, incomingProjected) };
    double incomingTheta { glm::orientedAngle(incomingDirection, incomingProjected, incomingProjected) },
           outgoingTheta { glm::orientedAngle(outgoingDirection, outgoingProjected, outgoingProjected) };

    double sigma2 { sigma };
    double A { 1.0 - 0.5*(sigma2 / (sigma2 + 0.33)) }, B { 0.45*(sigma2 / (sigma2 + 0.09)) };
    double alpha {  std::max(incomingTheta, outgoingTheta) }, beta { std::min(incomingTheta, outgoingTheta) };
    double azimuthStep { std::max(0.0, std::cos(incomingAzimuth - outgoingAzimuth)) };
    return albedo / glm::pi<double>() * (A + (B * azimuthStep) * std::sin(alpha) * std::tan(beta));
}
