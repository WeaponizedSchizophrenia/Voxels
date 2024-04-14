#pragma once

#include "CameraDescriptor.hpp"
#include "World/Entity.hpp"
#include "ext/vector_float3.hpp"
#include "gtc/constants.hpp"
#include "trigonometric.hpp"

namespace world {
    class Camera: public Entity {
    public:
        explicit Camera(glm::vec3 position, float32 fov = glm::radians(60.0f), 
            float32 yaw = -glm::half_pi<float32>(), float32 pitch = 0) noexcept
                : Entity(), position(position), yaw(yaw), pitch(pitch), fov(fov) {  }

        [[nodiscard]] inline glm::vec3 getDirection() const noexcept {
            auto pitchCos = glm::cos(pitch);
            return glm::normalize(glm::vec3{
                glm::cos(yaw) * pitchCos,
                glm::sin(pitch),
                glm::sin(yaw) * pitchCos
            });
        }
        [[nodiscard]] inline voxels::CameraDescriptor getDescriptor() const noexcept {
            return { position, getDirection(), fov };
        }
        [[nodiscard]] inline glm::mat3 constructViewDirectionMatrix() const noexcept {
            auto direction = getDirection();
            return { glm::cross(voxels::CameraDescriptor::UP, direction), voxels::CameraDescriptor::UP, direction };
        }

        float32 yaw; //< The yaw of the camera.
        float32 pitch; //< The pitch of the camera.
        float32 fov; //< The vertical FOV in radians.
        glm::vec3 position; //< The world position of the camera.
    };
}