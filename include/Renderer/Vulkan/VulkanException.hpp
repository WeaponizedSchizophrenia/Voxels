#pragma once

#include "Exception.hpp"

#include <sstream> // For message formatting
#include <vulkan/vulkan.hpp> // For vulkan result
#include <vulkan/vulkan_to_string.hpp> // For vulkan result to string

/// @brief Throws a vulkan exception with the provided message and vulkan result
#define THROW_VULKAN_EXCEPTION(msg, result) throw vulkan::VulkanException(__LINE__, __FILE__, msg, result)

namespace vulkan {
    /**
     * @brief Custom exception for vulkan errors.
     */
    class VulkanException : public voxels::Exception {
    public:
        explicit VulkanException(uint32 line, std::string&& file, std::string&& message, vk::Result result) noexcept
            : voxels::Exception(line, std::move(file), std::move(message)), m_result(result) {  }

    protected:
        [[nodiscard]] virtual std::string formatMessage() const noexcept override {
            std::stringstream sstream;
            // Format the message and specify the vulkan result.
            sstream << m_file << ':' << m_line << " VulkanException (" << vk::to_string(m_result) << "): " << m_message;
            return sstream.str();
        }

    private:
        vk::Result m_result; //< Vulkan result.
    };
}