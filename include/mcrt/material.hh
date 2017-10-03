#ifndef MCRT_MATERIAL_HH
#define MCRT_MATERIAL_HH

#include <glm/glm.hpp>

namespace mcrt {
    struct Material {
        enum class Type {
            Diffuse,
            Reflective,
            Refractive,
            LightSource
        };

        virtual ~Material() = default;
        Material(Type type, const glm::dvec3& color, double refractionIndex)
            : type { type }, color { color },
              refractionIndex { refractionIndex } {  }

        Type type;
        glm::dvec3 color;
        double refractionIndex;

        virtual glm::dvec3 brdf(const glm::dvec3&, const glm::dvec3&,
                                const glm::dvec3&, const glm::dvec3&) const = 0;
    };

    struct LambertianMaterial : public Material {
        LambertianMaterial(Type type, const glm::dvec3& color, double refractionIndex)
            : Material { type, color, refractionIndex} {  }

        glm::dvec3 brdf(const glm::dvec3&, const glm::dvec3&,
                        const glm::dvec3&, const glm::dvec3&) const override;
    };

    struct OrenNayarMaterial : public Material {
        OrenNayarMaterial(Type type, const glm::dvec3& color, double refractionIndex, double roughness)
            : Material { type, color, refractionIndex}, roughness { roughness } {  }

        double roughness;

        glm::dvec3 brdf(const glm::dvec3&, const glm::dvec3&,
                        const glm::dvec3&, const glm::dvec3&) const override;
    };
}

#endif
