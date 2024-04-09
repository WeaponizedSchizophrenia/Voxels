/**
 * @file Shader.cpp
 * @author WeaponizedSchizophrenia
 * @brief Contains the source for the Shader class.
 */

#include "Renderer/OpenGl/Shader.hpp" // For declarations.
#include "Renderer/CompilationException.hpp" // For exceptions.

renderer::opengl::Shader renderer::opengl::Shader::compileFromSource(std::string_view src, ShaderType shaderType) {
        auto openGlShaderType = [=] {
            switch(shaderType) {
                case ShaderType::Vertex: return GL_VERTEX_SHADER;
                case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
                default: return 0;
            }
        }();

        auto shader = glCreateShader(openGlShaderType);

        if(shader == 0) {
            THROW_EXCEPTION("Failed to create shader.");
        }

        // Set the shader source.
        auto srcData = src.data();
        glShaderSource(shader, 1, &srcData, nullptr);
        // Compile the shader.
        glCompileShader(shader);

        // Check if the compilation was successful.
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            char infoLog[INFO_LOG_BUFFER_SIZE];
            glGetShaderInfoLog(shader, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
            // Make sure to delete the shader if there was an error so that it doesn't leak.
            glDeleteShader(shader);
            THROW_COMP_EXCEPTION(shaderType, infoLog);
        }

        return renderer::opengl::Shader(shader);
}