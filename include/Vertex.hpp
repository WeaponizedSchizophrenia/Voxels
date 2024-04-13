#pragma once

#include "Renderer/AttributeDescriptor.hpp"
#include "ext/vector_float2.hpp"
#include "ext/vector_float3.hpp"
#include <array>

namespace voxels {
    struct Vertex {
        glm::vec3 localPosition;
        glm::vec2 textureCoords;

        [[nodiscard]] static constexpr auto getAttributes() noexcept {
            return std::array {
                renderer::AttributeDescriptor{ renderer::AttributeType::Float32, 3 },
                renderer::AttributeDescriptor{ renderer::AttributeType::Float32, 2 },
            };
        }
    };
}