#pragma once

#include "Exception.hpp"
#include "Renderer/IBindable.hpp"
#include "glad/glad.h"
#include <cstddef>

namespace renderer::opengl {
    class IBuffer: public IBindable {
    public: 
        using IBindable::bind, IBindable::unBind;

        /**
         * @brief Copies the given data into the buffer.
         * 
         * @param data The data to copy.
         * @param size The size of the data in bytes.
         */
        virtual void copyDataIntoBuffer(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW) noexcept = 0;
    };

    template<GLenum Type>
    class Buffer: public IBuffer {
    public:
        /**
         * @brief Construct a new buffer.
         * 
         * @throws voxels::Exception If the vertex array could not be created.
         */
        explicit Buffer() : m_buffer(0) {
            glGenBuffers(1, &m_buffer);
            if(m_buffer == 0) {
                THROW_EXCEPTION("Could not create a buffer.");
            }
        }
        explicit Buffer(GLuint buffer) noexcept : m_buffer(buffer) {  }
        virtual ~Buffer() noexcept {
            glDeleteBuffers(1, &m_buffer);
        }

        virtual void bind() const noexcept override {
            glBindBuffer(Type, m_buffer);
        }
        virtual void unBind() const noexcept override {
            glBindBuffer(Type, 0);
        }

        virtual void copyDataIntoBuffer(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW) noexcept override {
            bind();
            glBufferData(Type, size, data, usage);
            unBind();
        }

    protected:
        GLuint m_buffer;
    };

    using VertexBuffer = Buffer<GL_ARRAY_BUFFER>;
    using IndexBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER>;
}