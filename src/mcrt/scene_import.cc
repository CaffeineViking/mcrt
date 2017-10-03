#include "mcrt/scene_import.hh"

#include <fstream>
#include <unordered_map>
#include <stdexcept>
#include "json.hh"

#include "mcrt/ray.hh"
#include "mcrt/sphere.hh"
#include "mcrt/material.hh"
#include "mcrt/triangle.hh"
#include "mcrt/camera.hh"
#include "mcrt/mesh_import.hh"

mcrt::Scene mcrt::SceneImporter::load(const std::string& file) {
    std::ifstream fileStream { file };
    nlohmann::json parser;
    fileStream >> parser;
    mcrt::Scene scene;

    if (parser.find("camera") != parser.end()) {
        nlohmann::json camera { parser["camera"] };

        if (camera[0].find("origin") != camera[0].end()) {
            glm::dvec3 origin;
            origin.x = camera[0]["origin"][0].get<double>();
            origin.y = camera[0]["origin"][1].get<double>();
            origin.z = camera[0]["origin"][2].get<double>();
            scene.getCamera().moveTo(origin);
        }

        if (camera[0].find("fieldOfView") != camera[0].end()) {
            double toRadians { glm::pi<double>() / 180.0 };
            double fieldOfView { camera[0]["fieldOfView"].get<double>() };
            scene.getCamera().setFieldOfView(fieldOfView * toRadians);
        }

        glm::dvec3 lookAt { 0.0, 0.0, -1.0 },
                   upVector { 0.0, 1.0, .0 };
        if (camera[0].find("lookAt") != camera[0].end()) {
            lookAt.x = camera[0]["lookAt"][0].get<double>();
            lookAt.y = camera[0]["lookAt"][1].get<double>();
            lookAt.z = camera[0]["lookAt"][2].get<double>();
        }

        if (camera[0].find("up") != camera[0].end()) {
            upVector.x = camera[0]["up"][0].get<double>();
            upVector.y = camera[0]["up"][1].get<double>();
            upVector.z = camera[0]["up"][2].get<double>();
        }

        scene.getCamera().lookAt(lookAt, upVector);
    } else throw std::runtime_error { "Error: you must have a camera!" };

    if (parser.find("lights") != parser.end()) {
        nlohmann::json lights { parser["lights"] };

        for (auto light : lights[0]) {
            scene.add(PointLight {
                { light["origin"][0].get<double>(),
                  light["origin"][1].get<double>(),
                  light["origin"][2].get<double>() },
                { light["radiance"][0].get<double>(),
                  light["radiance"][1].get<double>(),
                  light["radiance"][2].get<double>() }
            });
        }
    }

    std::unordered_map<std::string, Material*> palette;

    auto stringToSurfaceType = [](const std::string& string) -> Material::Type {
        if (string == "diffuse") return Material::Type::DIFFUSE;
        else if (string == "reflective") return Material::Type::REFLECTIVE;
        else if (string == "refractive") return Material::Type::REFRACTIVE;
        else throw std::runtime_error { "Error: unkown surface type!" };
    };

    if (parser.find("materials") != parser.end()) {
        nlohmann::json materials { parser["materials"] };

        for (auto material : materials[0]) {
            std::string materialName { material["name"].get<std::string>() };
            if (palette.count(materialName) != 0) continue;

            std::string brdfType { material["brdf"].get<std::string>() };

            Material* materialPointer;
            if (brdfType == "lambertian") {
                materialPointer = new LambertianMaterial {
                    stringToSurfaceType(material["type"].get<std::string>()),
                    {
                        material["albedo"][0].get<double>(),
                        material["albedo"][1].get<double>(),
                        material["albedo"][2].get<double>()
                    },
                    material["refractionIndex"].get<double>()
                };
            } else if (brdfType == "oren-nayar") {
                materialPointer = new OrenNayarMaterial {
                    stringToSurfaceType(material["type"].get<std::string>()),
                    {
                        material["albedo"][0].get<double>(),
                        material["albedo"][1].get<double>(),
                        material["albedo"][2].get<double>()
                    },

                    material["refractionIndex"].get<double>(),
                    material["roughness"].get<double>()
                };
            } else throw std::runtime_error { "Error: unknown surface BRDF!" };

            palette[materialName] = materialPointer;
        }
    }

    for (const auto& material : palette)
        scene.add(material.second);

    if (parser.find("surfaces") != parser.end()) {
        nlohmann::json surfaces { parser["surfaces"] };
        for (auto surface : surfaces[0]) {
            std::string geometryType { surface["geometry"].get<std::string>() };
            std::string materialName { surface["material"].get<std::string>() };
            if (palette.count(materialName) == 0) // Do a simple check before going to town with this.
                throw std::runtime_error { "Error: the material '" + materialName + "' was not found!" };

            Geometry* geometry = nullptr;
            if (geometryType == "sphere") {
                geometry = new Sphere {
                    { surface["origin"][0].get<double>(),
                      surface["origin"][1].get<double>(),
                      surface["origin"][2].get<double>() },
                    surface["radius"].get<double>(),
                    palette[materialName]
                };
            } else if (geometryType == "triangle") {
                geometry = new Triangle {
                    { surface["vertex-1"][0].get<double>(),
                      surface["vertex-1"][1].get<double>(),
                      surface["vertex-1"][2].get<double>() },
                    { surface["vertex-2"][0].get<double>(),
                      surface["vertex-2"][1].get<double>(),
                      surface["vertex-2"][2].get<double>() },
                    { surface["vertex-3"][0].get<double>(),
                      surface["vertex-3"][1].get<double>(),
                      surface["vertex-3"][2].get<double>() },
                    palette[materialName]
                };
            } else if (geometryType == "mesh") {
                MeshImporter::setMaterial(palette[materialName]);
                Mesh* mesh = MeshImporter::load(surface["file"]);
                if (surface.find("scale") != surface.end())
                    mesh->scale(surface["scale"].get<double>());

                if (surface.find("rotate") != surface.end()) {
                    mesh->rotateX(surface["rotate"][0].get<double>());
                    mesh->rotateY(surface["rotate"][1].get<double>());
                    mesh->rotateZ(surface["rotate"][2].get<double>());
                }

                mesh->move({surface["position"][0].get<double>(),
                            surface["position"][1].get<double>(),
                            surface["position"][2].get<double>()});
                mesh->updateBoundingSphere();
                geometry = mesh;
            } else throw std::runtime_error { "Error: unknown geometry type '" + geometryType + "'!" };
            scene.add(geometry); // Actually add the surface to our scene. We need to destroy it later.
        }
    }

    return scene;
}
