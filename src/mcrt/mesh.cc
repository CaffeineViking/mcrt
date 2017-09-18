#include "mcrt/mesh.hh"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace mcrt {

    Mesh::Mesh() : Geometry(Material{{1.0, 1.0, 1.0}})
    {}
    
    Mesh::Mesh(const Material& m) :
	Geometry(m)
    {}

    void Mesh::move(glm::dvec3 p) {
	for (int i = 0; i < triangles.size(); i++) {
	    triangles[i].move(p);
	}
    }

    void Mesh::scale(const double& c) {
	for (int i = 0; i < triangles.size(); i++) {
	    triangles[i].scale(c);
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

    void Mesh::setMaterial(const Material& m) {
	_material = m;
    }

    void Mesh::addTriangle(const Triangle& t) {
	triangles.push_back(t);
    }

    void Mesh::addTriangle(glm::dvec3 v0, glm::dvec3 v1, glm::dvec3 v2){
	Triangle t{v0, v1, v2, _material};
	triangles.push_back(t);
    }

    std::vector<Triangle> Mesh::getTriangles() const {
	return triangles;
    }

    Intersection Mesh::intersect(const Ray& ray) const {
	Intersection res{0, glm::dvec3(), _material};
	for (auto t : triangles) {
	    Intersection i = t.intersect(ray);
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
	for (Triangle t : triangles) {
	    std::cout << t << std::endl;
	}
    }
}
