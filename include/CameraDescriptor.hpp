#pragma once

#include "Global.hpp"
#include "ext/matrix_float4x4.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/vector_float3.hpp"
#include "ext/vector_float4.hpp"
#include "geometric.hpp"

namespace voxels {
    struct CameraDescriptor {
        static constexpr auto UP = glm::vec3(0, 1, 0);

        const glm::vec3 position;     //< The position of the camera.
        const glm::vec3 direction;     //< The normalized looking direction of the camera.
        const float32 fieldOfView;    //< Vertical FOV in radians.

        CameraDescriptor(glm::vec3 pos, glm::vec3 direction, float32 fov) noexcept
            : position(pos), direction(direction), fieldOfView(fov) {  }

        [[nodiscard]] inline glm::mat4 constructViewMatrix() const noexcept {
            return glm::lookAtRH(position, position + direction, UP);
        }   
    };
}