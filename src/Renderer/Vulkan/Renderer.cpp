/**
 * @file Renderer.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the Vulkan Renderer.
 */

#include "Renderer/Vulkan/Renderer.hpp"
#include "Exception.hpp"
#include "Global.hpp"
#include "Renderer/Vulkan/VulkanException.hpp"

#include <cstring>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

// If it is a debug build than enable validation layers.
#ifndef NDEBUG
static constexpr const char* const LAYERS[] = {
    "VK_LAYER_KHRONOS_validation",
};

static constexpr const char* const EXTENSIONS[] = {
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

/**
 * @brief Checks if all the VALIDATION_LAYERS are supported.
 * 
 * @return const char* The validation layer that is not supported, if all are supported then nullptr is returned.
 */
static const char* areValidationLayersSupported() {
    // Check how many layers are supported.
    uint32 layerCount = 0;
    auto layerEnumResult = vk::enumerateInstanceLayerProperties(&layerCount, nullptr);
    if(layerEnumResult != vk::Result::eSuccess) {
        THROW_VULKAN_EXCEPTION("Failed to enumerate layer properties", layerEnumResult);
    }

    // Actually load the layers into memory.
    auto supportedValidationLayers = std::vector<vk::LayerProperties>(layerCount);
    layerEnumResult = vk::enumerateInstanceLayerProperties(&layerCount, supportedValidationLayers.data());
    if(layerEnumResult != vk::Result::eSuccess) {
        THROW_VULKAN_EXCEPTION("Failed to get layer properties", layerEnumResult);
    }

    // Enumerate through the VALIDATION_LAYERS and check if every single one is supported if not then return which is not.
    for(auto validationLayer : LAYERS) {
        auto foundLayer = false;

        for (const auto& supportedLayer: supportedValidationLayers) {
            if(std::strcmp(supportedLayer.layerName, validationLayer) == 0) {
                foundLayer = true;
                break;
            }
        }

        if(!foundLayer)
            return validationLayer;
    }

    return nullptr;
}

/**
 * @brief Debug message callback.
 * 
 * @param messageSeverity The message severity.
 * @param messageType The message type.
 * @param pCallbackData The callback data.
 * @param pUserData The user data.
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void* pUserData
) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

/**
 * @brief Loads the dynamic dispatch loader.
 * 
 * @param instance The instance.
 * @return vk::DispatchLoaderDynamic The dynamic dispatch loader.
 */
vk::DispatchLoaderDynamic getDynamicDispatchLoader(vk::Instance instance) {
    static vk::DispatchLoaderDynamic loader(instance, vkGetInstanceProcAddr);
    return loader;
}

#endif

vulkan::Renderer::Renderer(std::string_view applicationName, uint32 majorVersion, uint32 minorVersion, uint32 patchVersion)
    : m_context()
    , m_instance([&, this] {
        vk::ApplicationInfo appInfo {
            applicationName.data(),
            VK_MAKE_VERSION(majorVersion, minorVersion, patchVersion),
            ENGINE_NAME,
            VK_MAKE_VERSION(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH),
            VK_API_VERSION_1_0,
        };
        vk::InstanceCreateInfo instanceInfo {
            {},
            &appInfo
        };

        // If it is a debug build then set the validation layers.
        #ifndef NDEBUG

        const char* validationLayer = areValidationLayersSupported();
        // If there is a validation layer that is not supported then throw an exception.
        if(validationLayer) {
            std::stringstream sstream;
            sstream << validationLayer << " is not supported.";
            THROW_EXCEPTION(std::move(sstream.str()));
        }

        // Set the instance layers.
        instanceInfo.setEnabledLayerCount(std::size(LAYERS));
        instanceInfo.setPEnabledLayerNames(LAYERS);

        // Enable the instance extensions.
        instanceInfo.setEnabledExtensionCount(std::size(EXTENSIONS));
        instanceInfo.setPEnabledExtensionNames(EXTENSIONS);

        #endif

        return m_context.createInstance(instanceInfo);
    }())
    #ifndef NDEBUG
    , m_debugMessenger([this] {
        vk::DebugUtilsMessengerCreateInfoEXT debugInfo = {
            {},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
                | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
                | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding
                | vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            debugCallback,
            nullptr,
            nullptr
        };
        return m_instance.createDebugUtilsMessengerEXT(debugInfo);
    }())
    #endif
    , m_physicalDevice([this] {
        // Get all the physical devices.
        auto physicalDevices = vk::raii::PhysicalDevices(m_instance);
        // Throw if there are no devices that support Vulkan.
        if(physicalDevices.empty()) {
            THROW_EXCEPTION("Could not find any GPU that support Vulkan");
        }
        // For now just get the first available device.
        // In the future might implement an actual physical device selection.
        return physicalDevices.front();
    }())
    , m_device([this] {
        auto queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

        std::optional<uint32> graphicsQueueIndex = {  };
        for(uint32 i = 0; const auto& queueFamily : queueFamilyProperties) {
            if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsQueueIndex = i;
                break;
            }
            ++i;
        }

        if(!graphicsQueueIndex) {
            THROW_EXCEPTION("Could not find a queue family that supports graphics");
        }
        
        constexpr const float QUEUE_PRIORITIES[] = { 1.0f };

        vk::DeviceQueueCreateInfo queueCreateInfos[] = {
            {
                {},
                *graphicsQueueIndex,
                1,
                QUEUE_PRIORITIES,
                nullptr  
            },
        };

        vk::PhysicalDeviceFeatures deviceFeatures = m_physicalDevice.getFeatures();

        vk::DeviceCreateInfo deviceCreateInfo = {
            {},
            std::size(queueCreateInfos),
            queueCreateInfos,
            #ifndef NDEBUG
            std::size(LAYERS),
            LAYERS,
            #else
            // My GPU doesn't support VK_EXT_debug_utils, so this is left empty for now.
            0,
            nullptr,
            #endif
            0,
            nullptr,
            &deviceFeatures,
        };

        return m_physicalDevice.createDevice(deviceCreateInfo);
    }())    
{  }

vulkan::Renderer::~Renderer() noexcept {  }