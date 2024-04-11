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

        /**
         * @brief Copies the provided data into the vertex buffer.
         * 
         * @param data A pointer to the data.
         * @param size The size of the data in bytes.
         * @param usage How should OpenGl handle this data.
         */
        void copyDataIntoVertexBuffer(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW) noexcept {
            m_vertexBuffer.copyDataIntoBuffer(data, size, usage);
        }

        /**
         * @brief Copies the provided data into the index buffer.
         * 
         * @param data A pointer to the data.
         * @param size The size of the data in bytes.
         * @param usage How should OpenGl handle this data.
         */
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