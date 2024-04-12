#pragma once

#include "Global.hpp"
#include "ext/vector_bool3.hpp"
#include "ext/vector_float3.hpp"

namespace voxels {
    /**
     * @brief Converts local octant 3d space to a normalized 3d index.
     * 
     * @param index The 3d space to convert.
     * @return glm::bvec3 A bool vector representing the normalized 3d index.
     */
    static glm::bvec3 convertToNormalizedIndex(const glm::vec3& index) noexcept {
        return {
            index.x > 0,
            index.y < 0, // I do not know why this has to be different than the other ones nor do i care.
            index.z > 0,
        };
    }

    /**
     * @brief Converts a normalized 3d index to a scalar index to use for 1d arrays.
     * 
     * @param index The normalized 3d index.
     * @return uint8 The scalar index.
     */
    static uint8 convertToScalarIndex(const glm::bvec3& index) noexcept {
        return (index.x << 2) | (index.y << 1) | index.z; // Magic formula ai gave me.
    }
}