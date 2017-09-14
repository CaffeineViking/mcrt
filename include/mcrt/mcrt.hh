#ifndef MCRT_MCRT_HH
#define MCRT_MCRT_HH

#include <glm/glm.hpp>
#include <limits>
#include <vector>

namespace mcrt {

struct Ray {
    glm::dvec3 origin;
    glm::dvec3 direction;
};

struct Intersection {
    double distance;    // Distance to surface point
    glm::dvec3 normal;  // Normal of surface    
};

// Material struct, stores render information about a surface type.
class Material {
private:
    glm::dvec3 _color;    
public:
    Material(const glm::dvec3& c);
};

class Geometry {
private:
    Material _material;   

protected:
    Geometry(const Material& m);

public:
    virtual Intersection intersect(const Ray& ray) = 0;
};

// Parametric form representation of a sphere
class Sphere : public Geometry{
private:
    glm::dvec3 _origin;          
    double _radius;
public:
    Sphere(const glm::dvec3 o, double r, const Material& m);
    Intersection intersect(const Ray& ray) override;
};

class Triangle : public Geometry{
private:
    glm::dvec3 _v1;
    glm::dvec3 _v2;
    glm::dvec3 _v3;  
public:
    Triangle(const glm::dvec3& v1,const glm::dvec3& v2,const glm::dvec3& v3, const Material& m);
    Intersection intersect(const Ray& ray) override;    
};

}
#endif
