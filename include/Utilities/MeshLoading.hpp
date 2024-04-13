#pragma once

#include "Exception.hpp"
#include "Mesh.hpp"
#include "OBJ_Loader.h"
#include "Vertex.hpp"
#include <algorithm>
#include <filesystem>
#include <sstream>

namespace voxels {
    [[nodiscard]] static std::vector<Mesh> loadMeshes(std::filesystem::path path) {
        if(!std::filesystem::exists(path)) {
            std::stringstream sstream;
            sstream << path << " does not exist.";
            THROW_EXCEPTION(sstream.str());
        }

        if(path.filename().extension().compare(".obj") == 0) {
            objl::Loader loader;

            auto result = loader.LoadFile(path.c_str());
            if(!result) {
                THROW_EXCEPTION("Failed to load .obj file.");
            }

            std::vector<Mesh> meshes;
            meshes.reserve(loader.LoadedMeshes.size());

            std::ranges::transform(loader.LoadedMeshes, std::back_inserter(meshes), [](const objl::Mesh& mesh) {
                std::vector<Vertex> vertices;
                vertices.reserve(mesh.Vertices.size());

                std::ranges::transform(mesh.Vertices, std::back_inserter(vertices), [](const objl::Vertex& vertex) {
                    return Vertex {
                        {vertex.Position.X, vertex.Position.Y, vertex.Position.Z},
                        {vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y}
                    };
                });

                return Mesh { std::move(vertices), std::move(mesh.Indices) };
            });
            
            return meshes;
        } else {
            std::stringstream sstream;
            sstream << path.filename().extension() << " format is not supported.";
            THROW_EXCEPTION(sstream.str());
        }
    }
}