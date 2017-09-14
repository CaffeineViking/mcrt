#ifndef MCRT_SCENE_HH
#define MCRT_SCENE_HH

#include "mcrt.hh"
#include "geometry.hh"

namespace mcrt {
    
    class Scene {
    private:
	std::vector<const Geometry*> _geometry;
	std::vector<Light> _lights;
    public:
	Intersection intersect(const Ray& ray) const;
    
	void add(const Geometry& geometry);
	void add(Light light);
	glm::dvec3 rayTrace(const Ray& ray);    
    };

}
#endif
