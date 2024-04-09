/**
 * @file Renderer.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the OpenGl renderer implementation.
 */

#include "Renderer/OpenGl/Renderer.hpp" // For declarations.
#include "Exception.hpp" // For exceptions.
#include "File.hpp"
#include "Renderer/OpenGl/Pipeline.hpp"
#include "Renderer/OpenGl/Shader.hpp"
#include "Renderer/OpenGl/WindowVisitor.hpp" // For the visitor.
#include "glad/glad.h" // For glad functions.



// Temporary
void drawTriangle() {
    static auto pipeline = [] {
        auto vertSrcFuture = voxels::openForReading("/home/laurynas/Documents/Projects/Voxels/assets/shaders/basic.vert").readContent().get();
        auto fragSrcFuture = voxels::openForReading("/home/laurynas/Documents/Projects/Voxels/assets/shaders/basic.frag").readContent().get();

        auto vertex = renderer::opengl::Shader::compileFromSource(vertSrcFuture, renderer::ShaderType::Vertex);
        auto fragment = renderer::opengl::Shader::compileFromSource(fragSrcFuture, renderer::ShaderType::Fragment);
        
        return renderer::opengl::Pipeline::linkShaders(vertex, fragment);
    }();

    static constexpr const GLfloat VERTICES[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };

    static constexpr const GLuint INDICES[] = {
        0, 1, 2  
    };

    // Create the vao
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
        // Create the vbo
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);

        // Bind the vbo
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        // Copy data into the vbo
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_DYNAMIC_DRAW);

        // Set the vbo's attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(sizeof(GLfloat) * 3));
        glEnableVertexAttribArray(1);

        // Create the ebo
        GLuint indexBuffer;
        glGenBuffers(1, &indexBuffer);

        // Bind the ebo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        // Copy data into the ebo
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_DYNAMIC_DRAW);

    // Unbind the active vao
    glBindVertexArray(0);
    // Unbind the active vbo and ebo ( make sure that this is done after the vao so that the vao doesn't unbind the vbo and ebo )
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Drawing */

    // Bind the pipeline
    pipeline.bind();
    // Bind the vao
    glBindVertexArray(vertexArray);
    // Draw the triangle
    glDrawElements(GL_TRIANGLES, std::size(INDICES), GL_UNSIGNED_INT, nullptr);
    // Unbind the vao
    glBindVertexArray(0);

    // Cleanup
    pipeline.unBind();
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void drawSquare() {
    static auto pipeline = [] {
        auto vertSrcFuture = voxels::openForReading("/home/laurynas/Documents/Projects/Voxels/assets/shaders/basic.vert").readContent().get();
        auto fragSrcFuture = voxels::openForReading("/home/laurynas/Documents/Projects/Voxels/assets/shaders/basic.frag").readContent().get();

        auto vertex = renderer::opengl::Shader::compileFromSource(vertSrcFuture, renderer::ShaderType::Vertex);
        auto fragment = renderer::opengl::Shader::compileFromSource(fragSrcFuture, renderer::ShaderType::Fragment);
        
        return renderer::opengl::Pipeline::linkShaders(vertex, fragment);
    }();
    static constexpr const GLfloat VERTICES[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    static constexpr const GLuint INDICES[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    pipeline.bind();
    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, std::size(INDICES), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    pipeline.unBind();
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

renderer::opengl::Renderer::Renderer(const wnd::IWindow& window)
    : m_surface(window.acceptVisitor(renderer::opengl::WindowVisitor())) 
{
    m_surface->bind();
    if(!gladLoadGL()) {
        THROW_EXCEPTION("Could not load GL.");
    }
    m_surface->unBind();
}

void renderer::opengl::Renderer::render() {
    m_surface->bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawTriangle();
    drawSquare();

    m_surface->swapBuffers();
    
    m_surface->unBind();
}