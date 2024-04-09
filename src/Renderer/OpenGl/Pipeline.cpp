/**
 * @file Pipeline.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the Pipeline class.
 */

#include "Renderer/OpenGl/Pipeline.hpp" // For declarations.
#include "Renderer/CompilationException.hpp" // For INFO_LOG_BUFFER_SIZE.
#include "Renderer/LinkingException.hpp" // For exceptions.

renderer::opengl::Pipeline renderer::opengl::Pipeline::linkShaders(Shader vertexShader, Shader fragmentShader) {
    auto program = glCreateProgram();

    if(program == 0) {
        THROW_EXCEPTION("Could not create program.");
    }

    // Attach the shaders to the program.
    glAttachShader(program, vertexShader.get());
    glAttachShader(program, fragmentShader.get());

    // Link the program.
    glLinkProgram(program);

    // Check for errors.
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[INFO_LOG_BUFFER_SIZE];
        glGetProgramInfoLog(program, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
        // Make sure to cleanup the program if there was an error so that it doesn't leak.
        glDeleteProgram(program);
        THROW_LINK_EXCEPTION(infoLog);
    }

    return renderer::opengl::Pipeline(program, vertexShader, fragmentShader); 
}