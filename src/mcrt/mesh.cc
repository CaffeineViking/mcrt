#include "mcrt/mesh.hh"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace mcrt {
    glm::dvec3 getMin(MeshTriangle* t) {
        glm::dmat3 m{ t->getCorners() };
        double x = std::min(std::min(m[0][0], m[1][0]), m[2][0]);
        double y = std::min(std::min(m[0][1], m[1][1]), m[2][1]);
        double z = std::min(std::min(m[0][2], m[1][2]), m[2][2]);
        return {x, y, z};
    }

    glm::dvec3 getMax(MeshTriangle* t) {
        glm::dmat3 m{ t->getCorners() };
        double x = std::max(std::max(m[0][0], m[1][0]), m[2][0]);
        double y = std::max(std::max(m[0][1], m[1][1]), m[2][1]);
        double z = std::max(std::max(m[0][2], m[1][2]), m[2][2]);
        return {x, y, z};

    }


    Mesh::Mesh() : Geometry { nullptr } {  }
    Mesh::Mesh(Material* m) : Geometry { m } {  }

    void Mesh::move(glm::dvec3 p) {
        for (std::size_t i = 0; i < _triangles.size(); i++) {
            _triangles[i]->move(p);
        }
    }

    void Mesh::scale(const double& c) {
        for (std::size_t i = 0; i < _triangles.size(); i++) {
            _triangles[i]->scale(c);
        }
    }

    void Mesh::rotateX(const double& radius) {
        for (std::size_t i = 0; i < _triangles.size(); i++)
            _triangles[i]->rotate({1.0, 0.0, 0.0}, radius);
    }

    void Mesh::rotateY(const double& radius) {
        for (std::size_t i = 0; i < _triangles.size(); i++)
            _triangles[i]->rotate({0.0, 1.0, 0.0}, radius);
    }

    void Mesh::rotateZ(const double& radius) {
        for (std::size_t i = 0; i < _triangles.size(); i++)
            _triangles[i]->rotate({0.0, 0.0, 1.0}, radius);
    }

    void Mesh::updateBoundingSphere() {
        glm::dvec3 min { getMin(_triangles[0]) };
        glm::dvec3 max { getMax(_triangles[0]) };

        for (std::size_t i = 1; i < _triangles.size(); i++) {
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

    void Mesh::setMaterial(Material* m) {
        material = m;
    }

    void Mesh::addTriangle(MeshTriangle* t) {
        _triangles.push_back(t);
    }

    void Mesh::addTriangle(const glm::dvec3& v0, const glm::dvec3& v1, const glm::dvec3& v2,
                           const glm::dvec3& n0, const glm::dvec3& n1, const glm::dvec3& n2){
        MeshTriangle* t = new MeshTriangle{v0, v1, v2, n0, n1, n2, material};
        _triangles.push_back(t);
    }

    std::vector<MeshTriangle*> Mesh::getTriangles() const {
        return _triangles;
    }

    Ray::Intersection Mesh::intersect(const Ray& ray) const {
        Ray::Intersection boundIntersection = _bound.intersect(ray);
        if (boundIntersection.distance == 0)
            return boundIntersection;

        Ray::Intersection res{0, glm::dvec3(), material, glm::dvec3{}};
        for (auto t : _triangles) {
            Ray::Intersection i = t->intersect(ray);
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
        for (MeshTriangle* t : _triangles) {
            std::cout << *t << std::endl;
        }
    }
}
