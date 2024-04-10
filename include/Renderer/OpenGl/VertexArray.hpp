#pragma once

#include "Renderer/OpenGl/Buffer.hpp"
#include "Renderer/OpenGl/VertexBufferAttributes.hpp"

namespace renderer::opengl {
    class VertexArray: public IBindable {
    public:
        /**
         * @brief Construct a new vertex array object.
         * 
         * @throws voxels::Exception If the vertex array could not be created.
         */
        explicit VertexArray(VertexBufferAttributes&& attributes)
            : m_vao(0)
            , m_vertexBufferAttributes(std::move(attributes))
            , m_vertexBuffer(), m_indexBuffer()
        {
            glGenVertexArrays(1, &m_vao);
            if(m_vao == 0) {
                THROW_EXCEPTION("Could not create a vertex array.");
            }
            bind();
            m_vertexBuffer.bind();
            m_vertexBufferAttributes.bind();
            m_indexBuffer.bind();
            unBind();
            m_vertexBuffer.unBind();
            m_indexBuffer.unBind();
        }

        ~VertexArray() noexcept { glDeleteVertexArrays(1, &m_vao); }

        virtual void bind() const noexcept override {
            glBindVertexArray(m_vao);
        }
        virtual void unBind() const noexcept override {
            glBindVertexArray(0);
        }
        
        // /**
        //  * @brief Registers a single buffer to this vertex array.
        //  * 
        //  * @param buffer The buffer to register.
        //  *
        //  * @note If a buffer of the same type is already registered this will overwrite it.
        //  */
        // void registerBuffer(const IBuffer& buffer) const noexcept {
        //     this->bind();
        //     buffer.bind();
        //     this->unBind();
        //     buffer.unBind();
        // }

        // /**
        //  * @brief Registers multiple buffers to this vertex array.
        //  * 
        //  * @param buffers A span over a list of buffer pointers to register.
        //  *
        //  * @note If a buffer of the same type is already registered this will overwrite it.
        //  * @note This function does not take ownership of the buffer so the allocation is the responsibility of the caller.
        //  */
        // template<size_t N>
        // void registerBuffers(std::span<const IBuffer*, N> buffers) const noexcept {
        //     this->bind();
        //     for(auto buffer : buffers) {
        //         buffer->bind();
        //     }
        //     this->unBind();
        //     for(auto buffer : buffers) {
        //         buffer->unBind();
        //     }
        // }

        void copyDataIntoVertexBuffer(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW) noexcept {
            m_vertexBuffer.copyDataIntoBuffer(data, size, usage);
        }

        void copyDataIntoIndexBuffer(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW) noexcept {
            m_indexBuffer.copyDataIntoBuffer(data, size, usage);
        }

    private:
        GLuint m_vao;
        VertexBuffer m_vertexBuffer;
        IndexBuffer m_indexBuffer;
        VertexBufferAttributes m_vertexBufferAttributes;
    };
}