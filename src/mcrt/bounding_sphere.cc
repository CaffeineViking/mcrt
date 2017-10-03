#include "mcrt/bounding_sphere.hh"

namespace mcrt {
    BoundingSphere::BoundingSphere(const glm::dvec3& origin, const double& radius) : _origin(origin), _radius(radius) {}

    Ray::Intersection BoundingSphere::intersect(const Ray& ray) const {
        Ray::Intersection result {0, glm::dvec3(0.0), nullptr, glm::dvec3(0.0)};

        double t0,t1;
        glm::dvec3 L = _origin - ray.origin;
        double tca = glm::dot(L,ray.direction);
        if(tca < 0) {
            return result;            
        }
	
        double d2 = glm::dot(L,L) - tca * tca;
        if( d2 > _radius*_radius) {
            return result;
        }

        double thc = std::sqrt(_radius*_radius -d2);
        t0 = tca - thc;
        t1 = tca + thc;

        if(t0 > t1) {
            double tmp = t0;
            t0 = t1;
            t1 = tmp;
        }

        if(t0 < 0){
            t0 = t1;
            if( t0 < 0) {
                return result;
            }
        }

        result.distance = t0;
        result.normal = glm::normalize((ray.origin + ray.direction * t0) - _origin);
        return result;
    }
}
