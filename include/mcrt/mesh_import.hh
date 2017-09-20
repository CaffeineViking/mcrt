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
        static Mesh* load(std::string);
        static void setMaterial(const Material&);
	
    private:
        static Material _material;
    };
}

#endif
