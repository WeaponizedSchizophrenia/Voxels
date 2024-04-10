/**
 * @file Renderer.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the OpenGl renderer implementation.
 */

#include "Renderer/OpenGl/Renderer.hpp" // For declarations.
#include "File.hpp"
#include "Renderer/OpenGl/Buffer.hpp"
#include "Renderer/OpenGl/Pipeline.hpp"
#include "Renderer/OpenGl/Shader.hpp"
#include "Renderer/OpenGl/VertexArray.hpp"
#include "Renderer/OpenGl/VertexBufferAttributes.hpp"
#include "Renderer/OpenGl/WindowVisitor.hpp" // For the visitor.
#include <iostream>

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

    auto vertexArray = renderer::opengl::VertexArray({
        { GL_FLOAT, 3 },
        { GL_FLOAT, 3 }
    });

    vertexArray.copyDataIntoVertexBuffer(VERTICES, sizeof(VERTICES));
    vertexArray.copyDataIntoIndexBuffer(INDICES, sizeof(INDICES));

    pipeline.bind();
    vertexArray.bind();
    glDrawElements(GL_TRIANGLES, std::size(INDICES), GL_UNSIGNED_INT, nullptr);
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

    auto vertexArray = renderer::opengl::VertexArray({
        { GL_FLOAT, 3 },
        { GL_FLOAT, 3 }
    });

    vertexArray.copyDataIntoVertexBuffer(VERTICES, sizeof(VERTICES));
    vertexArray.copyDataIntoIndexBuffer(INDICES, sizeof(INDICES));

    pipeline.bind();
    vertexArray.bind();
    glDrawElements(GL_TRIANGLES, std::size(INDICES), GL_UNSIGNED_INT, nullptr);
}

renderer::opengl::Renderer::Renderer(const wnd::IWindow& window)
    : m_surface(window.acceptVisitor(renderer::opengl::WindowVisitor())) 
{
    m_surface->bind();
    if(!gladLoaderLoadGL()) {
        THROW_EXCEPTION("Could not load GL.");
    }

    std::cout << glGetString(GL_VERSION) << '\n';

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