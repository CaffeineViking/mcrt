#include "mcrt/mesh.hh"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace mcrt {
    
    glm::dvec3 getMin(Triangle* t) {
        glm::dmat3 m{ t->getCorners() };
        double x = std::min(std::min(m[0][0], m[1][0]), m[2][0]);
        double y = std::min(std::min(m[0][1], m[1][1]), m[2][1]);
        double z = std::min(std::min(m[0][2], m[1][2]), m[2][2]);
        return {x, y, z};
    }

    glm::dvec3 getMax(Triangle* t) {
        glm::dmat3 m{ t->getCorners() };
        double x = std::max(std::max(m[0][0], m[1][0]), m[2][0]);
        double y = std::max(std::max(m[0][1], m[1][1]), m[2][1]);
        double z = std::max(std::max(m[0][2], m[1][2]), m[2][2]);
        return {x, y, z};

    }


    Mesh::Mesh() : Geometry(Material{{1.0, 1.0, 1.0}})
    {}
    
    Mesh::Mesh(const Material& m) :
        Geometry(m)
    {}

    void Mesh::move(glm::dvec3 p) {
        for (int i = 0; i < _triangles.size(); i++) {
            _triangles[i]->move(p);
        }
    }

    // May not work as intended
    void Mesh::scale(const double& c) {
        for (int i = 0; i < _triangles.size(); i++) {
            _triangles[i]->scale(c);
        }
    }

    void Mesh::rotateX(const double& c) {
        //TODO
    }

    void Mesh::rotateY(const double& c) {
        //TODO
    }

    void Mesh::rotateZ(const double& c) {
        //TODO
    }

    void Mesh::updateBoundingSphere() {
        glm::dvec3 min { getMin(_triangles[0]) };
        glm::dvec3 max { getMax(_triangles[0]) };

        for (int i = 1; i < _triangles.size(); i++) {
            auto t = _triangles[i];
            glm::dvec3 min2 { getMin(t) };
            glm::dvec3 max2 { getMax(t) };
            min[0] = std::min(min[0], min2[0]);
            min[1] = std::min(min[1], min2[1]);
            min[2] = std::min(min[2], min2[2]);
            max[0] = std::max(max[0], max2[0]);
            max[1] = std::max(max[1], max2[1]);
            max[2] = std::max(max[2], max2[2]);
        }

        double radius = glm::distance(min,max)/2;
        glm::dvec3 origin = glm::mix(min, max, 0.5);
        _bound = BoundingSphere{origin, radius};
    }

    void Mesh::setMaterial(const Material& m) {
        _material = m;
    }

    void Mesh::addTriangle(Triangle* t) {
        _triangles.push_back(t);
    }

    void Mesh::addTriangle(glm::dvec3 v0, glm::dvec3 v1, glm::dvec3 v2){
        Triangle* t = new Triangle{v0, v1, v2, _material};
        _triangles.push_back(t);
    }

    std::vector<Triangle*> Mesh::getTriangles() const {
        return _triangles;
    }

    Intersection Mesh::intersect(const Ray& ray) const {
        Intersection boundIntersection = _bound.intersect(ray);
        if (boundIntersection.distance == 0)
            return boundIntersection;

        Intersection res{0, glm::dvec3(), _material};
        for (auto t : _triangles) {
            Intersection i = t->intersect(ray);
            if (i.distance == 0) continue;
            if (res.distance == 0) {
                res = i;
            } else if (i.distance < res.distance) {
                res = i;
            }
        }
        return res;
    }

    void Mesh::print() {
        for (Triangle* t : _triangles) {
            std::cout << *t << std::endl;
        }
    }
}
