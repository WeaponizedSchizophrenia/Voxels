#pragma once

#include "Global.hpp"
#include "Renderer/IBindable.hpp"
#include "Renderer/OpenGl/Common.hpp"
#include "Renderer/AttributeDescriptor.hpp"
#include <initializer_list>
#include <vector>

namespace renderer::opengl {
    /**
     * @brief Holds and manages vertex buffer attributes.
     */
    class VertexBufferAttributes: public IBindable {
    public:
        constexpr explicit VertexBufferAttributes() noexcept = default;
        constexpr explicit VertexBufferAttributes(uint32 attributeCount) noexcept {
            m_attributes.reserve(attributeCount);
        }
        template<size_t N>
        constexpr VertexBufferAttributes(std::array<AttributeDescriptor, N>&& attributes) noexcept
            : m_attributes(attributes.begin(), attributes.end())
        {
            for(const auto& attribute : m_attributes) {
                m_stride += attribute.count * getSizeOfType(attribute.type);
            }
        }
        constexpr VertexBufferAttributes(std::initializer_list<AttributeDescriptor> attributes) noexcept
            : m_attributes(attributes) 
        {
            for(const auto& attribute : m_attributes) {
                m_stride += attribute.count * getSizeOfType(attribute.type);
            }
        }

        /**
         * @brief Adds an attribute to the vertex buffer.
         * 
         * @param attribute The attribute descriptor to add.
         */
        constexpr void addAttribute(AttributeDescriptor attribute) noexcept {
            m_stride += attribute.count * getSizeOfType(attribute.type);
            m_attributes.emplace_back(attribute);
        }

        constexpr virtual void bind() const noexcept override {
            auto currentSize = 0u;
            for(int i = 0; i < m_attributes.size(); ++i) {
                glVertexAttribPointer(i, m_attributes[i].count, fromAttributeType(m_attributes[i].type), m_attributes[i].normalized, 
                    m_stride, reinterpret_cast<void*>(currentSize));
                currentSize += m_attributes[i].count * getSizeOfType(m_attributes[i].type);
                glEnableVertexAttribArray(i);
            }
        }

        constexpr virtual void unBind() const noexcept override {
            for(int i = 0; i < m_attributes.size(); ++i) {
                glDisableVertexAttribArray(i);
            }
        }

    private:
        std::vector<AttributeDescriptor> m_attributes; //< A vector of attribute descriptors.
        uint32 m_stride = 0; //< The current stride of all the attributes.
    };
}