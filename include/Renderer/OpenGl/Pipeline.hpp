#pragma once

#include "Renderer/OpenGl/Shader.hpp" // For the shader.
#include "Renderer/IBindable.hpp" // For the interface.
#include "gtc/type_ptr.hpp"
#include <string_view>

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

        /**
         * @brief Sets a uniform in the pipeline.
         * 
         * @param name The name of the uniform.
         * @param value The value of the uniform.
         *
         * @note This function unbinds this pipeline after it is done.
         */
        void setUniform(std::string_view name, float value) const noexcept {
            bind();
            glUniform1f(glGetUniformLocation(m_programId, name.data()), value);
            unBind();
        }

        /**
         * @brief Sets a uniform in the pipeline.
         * 
         * @param name The name of the uniform.
         * @param value The value of the uniform.
         *
         * @note This function unbinds this pipeline after it is done.
         */
        void setUniform(std::string_view name, glm::vec3 value) const noexcept {
            bind();
            glUniform3fv(glGetUniformLocation(m_programId, name.data()), 1, glm::value_ptr(value));
            unBind();
        }

        /**
         * @brief Sets a uniform in the pipeline.
         * 
         * @param name The name of the uniform.
         * @param value The value of the uniform.
         *
         * @note This function unbinds this pipeline after it is done.
         */
        void setUniform(std::string_view name, glm::mat4 value) const noexcept {
            bind();
            glUniformMatrix4fv(glGetUniformLocation(m_programId, name.data()), 1, GL_FALSE, glm::value_ptr(value));
            unBind();
        }

    private:
        GLuint m_programId; //< Program ID.
        Shader m_vertexShader; //< Vertex shader.
        Shader m_fragmentShader; //< Fragment shader.
    };
}