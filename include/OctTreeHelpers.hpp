#pragma once

#include "Global.hpp"
#include "ext/vector_bool3.hpp"
#include "ext/vector_int3.hpp"

namespace voxels {
    static glm::bvec3 convertToNormalizedIndex(const glm::ivec3& index, const uint32 length) noexcept {
        return {
            index.x >= (length / 2),
            index.y >= (length / 2),
            index.z >= (length / 2)  
        };
    }

    static uint8 convertToScalarIndex(const glm::bvec3& index) noexcept {
        return (index.x << 2) | (index.y << 1) | index.z;
    }
}