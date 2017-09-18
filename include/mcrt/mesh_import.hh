#ifndef MCRT_MESH_IMPORTER
#define MCRT_MESH_IMPORTER

#include "triangle.hh"
#include <vector>
#include <string>
#include <iostream>
#include "mcrt.hh"
#include "mesh.hh"

namespace mcrt {

    class MeshImporter {
    public:
	MeshImporter();
    
	bool load(std::string);
	void setMaterial(const Material&);
	Mesh getMesh();
	
    private:
	Material _material;
	Mesh _mesh;
    };
}

#endif
