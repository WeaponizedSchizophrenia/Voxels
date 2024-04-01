#pragma once

#include "Global.hpp"
#include <string_view>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_raii.hpp>


namespace vulkan {
    class Renderer {
    public:
        /**
         * @brief Constructs a new Vulkan Renderer.
         * 
         * @throws vulkan::VulkanException If the Vulkan instance could not be created.
         * @throws vulkan::VulkanException If it is a debug build and validation layers could not be enumerated or gotten.
         * @throws voxels::Exception If none of the physical devices support Vulkan.
         * @throws voxels::Exception If it is a debug build and the validation layers are not supported.
         * @throws std::system_error Exception from vulkan calls. ( It does not specifically throw std::system_error, but it is a base class for all vulkan exceptions.)
         */
        explicit Renderer(std::string_view applicationName, uint32 majorVersion = 0, uint32 minorVersion = 0, uint32 patchVersion = 1);
        ~Renderer() noexcept;

    private:
        vk::raii::Context m_context; //< Vulkan context.
        vk::raii::Instance m_instance; //< Vulkan instance.
        vk::raii::PhysicalDevice m_physicalDevice; //< Physical device.
        vk::raii::Device m_device; //< Logical device.
        // If it is a debug build then store the debug messenger handler.
        #ifndef NDEBUG
        vk::raii::DebugUtilsMessengerEXT m_debugMessenger; //< Vulkan debug messenger.
        #endif
    };
}