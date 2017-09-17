#ifndef MCRT_SCENE_HH
#define MCRT_SCENE_HH

#include "mcrt/mcrt.hh"
#include "mcrt/camera.hh"
#include "mcrt/geometry.hh"

namespace mcrt {
    class Scene {
    public:
        Intersection intersect(const Ray& ray) const;

        void add(const Light& light);
        void add(const Geometry& geometry);

        glm::dvec3 rayTrace(const Ray& ray) const;

        const Camera& getCamera() const { return camera; }
        Camera& getCamera() { return camera; }

    private:
        std::vector<const Geometry*> geometries;
        std::vector<Light> lights;
        Camera camera;
    };
}

#endif
