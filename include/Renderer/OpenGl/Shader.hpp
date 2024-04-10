#pragma once

#include "Renderer/ShaderType.hpp" // For the shader type.
#include "glad/gl.h" // For glad functions.
#include <string_view> // For string view.

namespace renderer::opengl {
    /**
     * @brief OpenGl shader RAII wrapper.
     */
    class Shader {
    public:
        explicit Shader(GLuint shader) noexcept : m_shader(shader) {  }

        ~Shader() noexcept { glDeleteShader(m_shader); }

        /**
         * @brief Compiles a shader from source.
         * 
         * @param src The shader source.
         * @param shaderType The shader type.
         * @return Shader The compiled shader.
         *
         * @throws voxels::CompilationException If the shader could not be compiled.
         */
        [[nodiscard]] static Shader compileFromSource(std::string_view src, ShaderType shaderType);

        /**
         * @brief Returns the underlying OpenGl shader id.
         * 
         * @return GLuint Shader id.
         */
        [[nodiscard]] inline GLuint get() const noexcept { return m_shader; }

    private:
        GLuint m_shader; //< The underlying OpenGl shader id.
    };
}
