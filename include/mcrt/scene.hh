#ifndef MCRT_SCENE_HH
#define MCRT_SCENE_HH

#include "mcrt/ray.hh"
#include "mcrt/lights.hh"
#include "mcrt/camera.hh"
#include "mcrt/geometry.hh"
#include "mcrt/photon.hh"
#include "mcrt/photon_map.hh"

namespace mcrt {
    class Light;
    class Scene {
    public:
        Scene() = default;

        ~Scene() {
            for (auto m : materials)  delete m;
            for (auto g : geometries) delete g;
        }

        Scene& operator=(const Scene&) = delete;
        Scene(const Scene&) = delete;

        Scene(Scene&& other) noexcept {
            *this = std::move(other);
        }

        Scene& operator=(Scene&& other) noexcept {
            camera = other.camera;
            lights = other.lights;

            // Here comes the trick, rip this classes' guts out!
            for (size_t i { 0 }; i < other.geometries.size(); ++i) {
                geometries.push_back(other.geometries[i]);
                other.geometries[i] = nullptr;
            }

            for (size_t i { 0 }; i < other.materials.size(); ++i) {
                materials.push_back(other.materials[i]);
                other.materials[i] = nullptr;
            }

            return *this;
        }

        void add(Material* material);
        void add(Light* light);
        void add(Geometry* geometry);

        glm::dvec3 rayTrace(const Ray& ray, const size_t) const;
        Ray::Intersection intersect(const Ray& ray) const;

        double inShadow(const Ray& ray) const;

        void gatherPhotons(std::size_t);

        std::vector<Material*>& getMaterials() { return materials; }
        const std::vector<Material*>& getMaterials() const { return materials; }

        std::vector<Geometry*>& getGeometries() { return geometries; }
        const std::vector<Geometry*>& getGeometries() const { return geometries; }

        static size_t maxRayDepth;
        static double photonEstimationRadius;

        const std::vector<Light*>& getLights() const { return lights; }
        std::vector<Light*>& getLights() { return lights; }

        const Camera& getCamera() const { return camera; }
        Camera& getCamera() { return camera; }

    private:
        std::vector<Material*> materials;
        std::vector<Geometry*> geometries;
        bool photonMapEnabled { false };
        std::vector<Light*> lights;
        PhotonMap photonMap;

        unsigned currentPhoton;

        bool photonTrace(const Ray& ray, const glm::dvec3&, const size_t);
        void getPhotons(const Ray& ray, const glm::dvec3&);
        bool hasPhotonMap() { return photonMapEnabled;}

        Camera camera;
    };
}

#endif
