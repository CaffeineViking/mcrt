#ifndef MCRT_MATERIAL_HH
#define MCRT_MATERIAL_HH

#include <glm/glm.hpp>

namespace mcrt {
    struct Material {
        enum Type {
            Diffuse     = 0,
            Reflective  = 1,
            Refractive  = 2
        };

        virtual ~Material() = default;

        Material(const glm::dvec3& albedo, Type type, double refractionIndex)
            : albedo { albedo }, type { type}, refractionIndex { refractionIndex } {  }

        glm::dvec3 albedo;
        Type type;
        double refractionIndex;

        glm::dvec3 brdf(const glm::dvec3& normal) const { return brdf(normal, {}, {}); }
        virtual glm::dvec3 brdf(const glm::dvec3&, const glm::dvec3&, const glm::dvec3&) const = 0;
    };

    struct LambertianMaterial : public Material {
        using Material::Material;
        glm::dvec3 brdf(const glm::dvec3&, const glm::dvec3&, const glm::dvec3&) const override;
    };

    struct OrenNayarMaterial : public Material {
        double sigma;
        OrenNayarMaterial(const glm::dvec3& color, Type type, double refractionIndex, double sigma)
            : Material { color, type, refractionIndex }, sigma { sigma } {  }
        glm::dvec3 brdf(const glm::dvec3&, const glm::dvec3&, const glm::dvec3&) const override;
    };
}

#endif
