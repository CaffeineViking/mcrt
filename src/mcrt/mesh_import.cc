#include "mcrt/mesh_import.hh"
#include <vector>
#include <tiny_obj_loader.hh>
#include <glm/glm.hpp>
#include <string>
#include <iostream>

namespace mcrt {
    Material MeshImporter::_material { Material{{1.0,1.0,1.0}} };

    //    MeshImporter::MeshImporter() {
    //  _material = Material{{1.0, 1.0, 1.0}};
    //}

    Mesh* MeshImporter::load(std::string filename) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
  
        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());
    
        if (!err.empty()) { // `err` may contain warning message.
            std::cerr << err << std::endl;
        }

        Mesh* mesh = new Mesh{_material};

        if (!ret) {
            return mesh;
        }        

	
        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                unsigned fv = shapes[s].mesh.num_face_vertices[f];

                std::vector<glm::dvec3> vertices;
	    
                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                    tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                    tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                    //tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                    //tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                    //tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
                    //tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
                    //tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];

                    vertices.push_back({vx, vy, vz});
                }
                if (vertices.size() == 3) {
                    mesh->addTriangle(vertices[0], vertices[1], vertices[2]);
                }
                index_offset += fv;
            }
        }
        return mesh;
    }
    
    void MeshImporter::setMaterial(const Material& m) {
        _material = m;
    }
        
}
