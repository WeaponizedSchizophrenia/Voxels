#pragma once

#include "Global.hpp"
#include "Vertex.hpp"
#include <vector>

namespace voxels {
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32> indices;

        [[nodiscard]] inline uint32 getIndexCount() const noexcept { return indices.size(); }
    };
}