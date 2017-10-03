#ifndef MCRT_MESH_IMPORT_HH
#define MCRT_MESH_IMPORT_HH

#include <vector>
#include <string>
#include <iostream>

#include "mcrt/triangle.hh"
#include "mcrt/mesh.hh"

namespace mcrt {
    class MeshImporter {
    public:
        static Mesh* load(std::string);
        static void setMaterial(Material*);

    private:
        static Material* _material;
    };
}

#endif
