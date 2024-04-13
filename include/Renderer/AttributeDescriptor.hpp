#pragma once

#include "Exception.hpp"
#include "Global.hpp"
namespace renderer {
    enum class AttributeType : uint8 {
        Float32,
        Integer32,
        Byte,
        UnsignedInteger32,
    };

    [[nodiscard]] static inline constexpr auto getSizeOfType(AttributeType type) {
        switch(type) {
            case AttributeType::Float32:            return sizeof(float32);
            case AttributeType::Integer32:          return sizeof(int32);
            case AttributeType::Byte:               return sizeof(int8);
            case AttributeType::UnsignedInteger32:  return sizeof(uint32);
            default: THROW_EXCEPTION("Parameter [type] is not a valid vertex attribute type");
        }
    }

    /**
     * @brief Describes a single vertex buffer attribute.
     */
    struct AttributeDescriptor {
        AttributeType type;         //< The type of the attribute.
        uint8 count;                //< The number of the specified type.
        bool normalized = false;    //< Whether the attribute should be normalized.
    };
}