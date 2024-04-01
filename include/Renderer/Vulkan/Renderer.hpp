#pragma once

#include "Global.hpp"
#include <string_view>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_shared.hpp>

namespace vulkan {
    class Renderer {
    public:
        /**
         * @brief Constructs a new Vulkan Renderer.
         * 
         * @throws vulkan::VulkanException If the Vulkan instance could not be created.
         * @throws vulkan::VulkanException If it is a debug build and validation layers could not be enumerated or gotten.
         * @throws voxels::Exception If it is a debug build and the validation layers are not supported.
         */
        explicit Renderer(std::string_view applicationName, uint32 majorVersion = 0, uint32 minorVersion = 0, uint32 patchVersion = 1);
        ~Renderer() noexcept;

    private:
        vk::Instance m_instance; //< Vulkan instance.
        // If it is a debug build then store the debug messenger handler.
        #ifndef NDEBUG
        vk::DebugUtilsMessengerEXT m_debugMessenger; //< Vulkan debug messenger.
        #endif
    };
}