#pragma once

#include "Global.hpp"
#include "ext/matrix_float4x4.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/vector_float3.hpp"
#include "geometric.hpp"

namespace voxels {
    struct CameraDescriptor {
        static constexpr auto UP = glm::vec3(0, 1, 0);

        glm::vec3 position;     //< The position of the camera.
        float32 pitch;          //< The pitch of the camera.
        float32 yaw;            //< The yaw of the camera.
        float32 fieldOfView;    //< Vertical FOV in radians.

        CameraDescriptor(glm::vec3 pos, float32 pitch, float32 yaw, float32 fov) noexcept
            : position(pos), pitch(pitch), yaw(yaw), fieldOfView(fov) {  }

        [[nodiscard]] inline glm::vec3 getDirection() const noexcept {
            auto pitchCos = std::cos(pitch);
            return glm::normalize(glm::vec3{
                std::cos(yaw) * pitchCos,
                std::sin(pitch),
                std::sin(yaw) * pitchCos,
            });
        }

        [[nodiscard]] inline glm::mat4 constructViewMatrix() const noexcept {
            return glm::lookAtRH(position, position + getDirection(), UP);
        }

        [[nodiscard]] inline glm::mat3 constructViewDirectionMatrix() const noexcept {
            auto dir = getDirection();
            return { glm::cross(dir, UP), UP, dir };
        }        
    };
}