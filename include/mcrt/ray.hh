#ifndef MCRT_RAY_HH
#define MCRT_RAY_HH

#include <cmath>
#include <vector>
#include <limits>

#include <glm/glm.hpp>

#include "mcrt/material.hh"

namespace mcrt {
    struct Ray {
        // Used for e.g. offsetting ray fr. surf.
        static constexpr double EPSILON { 1e-8 };

        struct Intersection {
            double distance; // Distance to the surface's point.
            glm::dvec3 normal; // Normal of surface intersection.
            Material material; // Material of a intersect point.

            glm::dvec3 sampleHemisphere() const {
                
            }
        };

        double fresnel(const glm::dvec3&, const double) const;
        Ray reflect(const glm::dvec3&, const glm::dvec3&) const;
        // Special case of reflect where we are inside our surface.
        Ray insideReflect(const glm::dvec3&, const glm::dvec3&) const;
        Ray refract(const glm::dvec3&, const glm::dvec3&, double) const;

        glm::dvec3 origin;
        glm::dvec3 direction;
    };
}

inline double mcrt::Ray::fresnel( const glm::dvec3& normal, const double refractionIndex) const {
        double cosi = glm::clamp(glm::dot(direction, normal),-1.0,1.0);
        double etai = 1;
        double etat = refractionIndex;
        if(cosi > 0) {
            std::swap(etai,etat);
        }

        double sint = etai / etat * std::sqrt(std::max(0.0,1.0 - cosi * cosi));
        if(sint >= 1.0) {
            return 1;
        }
        else {
            double cost = std::sqrt(std::max(0.0,1 - sint * sint));
            cosi = std::abs(cosi);
            double Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            double Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            return (Rs * Rs + Rp * Rp) / 2.0;
        }
    }

    // Perfect reflection
inline mcrt::Ray mcrt::Ray::reflect(const glm::dvec3& hitPosition, const glm::dvec3& surfaceNormal) const {
    glm::dvec3 reflection { direction - 2.0*surfaceNormal * glm::dot(direction, surfaceNormal) };
    glm::dvec3 normalizedReflection { glm::normalize(reflection) };
    return { hitPosition + normalizedReflection*EPSILON, normalizedReflection };
}

    // Perfect refraction from high density to air
inline mcrt::Ray mcrt::Ray::insideReflect(const glm::dvec3& hitPosition, const glm::dvec3& surfaceNormal) const {
    glm::dvec3 reflection { direction - 2.0*surfaceNormal * glm::dot(direction, surfaceNormal) };
    glm::dvec3 normalizedReflection { glm::normalize(reflection) };
    return { hitPosition + normalizedReflection*-EPSILON, normalizedReflection };
}

    // Perfect refraction from air to high density
inline mcrt::Ray mcrt::Ray::refract(const glm::dvec3& hitPosition, const glm::dvec3& surfaceNormal,
                             double refractionIndex) const {
    double cosi = glm::clamp(glm::dot(surfaceNormal, direction), -1.0, 1.0);
    bool outside = glm::dot(direction, surfaceNormal) < 0.0;
    glm::dvec3 bias = surfaceNormal*Ray::EPSILON;

    double n1 = 1.0;
    double n2 = refractionIndex;
    glm::dvec3 normal = surfaceNormal;
    glm::dvec3 refractionDir;

    if(cosi < 0.0)
        cosi = -cosi;
    else {
        std::swap(n1,n2);
        normal = -normal;
    }

    double n = n1 / n2;
    double k = 1.0 - n * n * (1.0 - cosi * cosi);

    if(k < 0.0)
        refractionDir = glm::dvec3(0.0);
    else
        refractionDir = n * direction + (n * cosi - glm::sqrt(k)) * normal;
    return { hitPosition + (outside ? -bias : bias), refractionDir };
}

#endif
