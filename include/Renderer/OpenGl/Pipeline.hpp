#pragma once

#include "Renderer/OpenGl/Shader.hpp" // For the shader.
#include "Renderer/IBindable.hpp" // For the interface.

namespace renderer::opengl {
    /**
     * @brief Holds the OpenGL rendering pipeline ( aka the program ).
     */
    class Pipeline : IBindable {
    public:
        explicit Pipeline(GLuint programId, Shader vertexShader, Shader fragmentShader) noexcept
            : m_programId(programId)
            , m_vertexShader(std::move(vertexShader))
            , m_fragmentShader(std::move(fragmentShader)) {  }

        virtual ~Pipeline() noexcept {
            unBind();
            glDeleteProgram(m_programId);
        }

        /**
         * @brief Creates a program by linking the given shaders.
         * 
         * @param vertexShader The pipeline's vertex shader.
         * @param fragmentShader The pipeline's fragment shader.
         * @return Pipeline The pipeline.
         */
        [[nodiscard]] static Pipeline linkShaders(Shader vertexShader, Shader fragmentShader);

        virtual void bind() const noexcept override {
            glUseProgram(m_programId);
        }

        virtual void unBind() const noexcept override {
            glUseProgram(0);   
        }

    private:
        GLuint m_programId; //< Program ID.
        Shader m_vertexShader; //< Vertex shader.
        Shader m_fragmentShader; //< Fragment shader.
    };
}