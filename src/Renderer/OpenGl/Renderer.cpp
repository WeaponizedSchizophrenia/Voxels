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
#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"
#include <iostream>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

static constexpr const Vertex VERTICES[] = {
    {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
    {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)}
};

static constexpr const GLuint INDICES[] = {
    0, 1, 2, 2, 1, 3,
    1, 5, 3, 3, 5, 7,
    5, 4, 7, 7, 4, 6,
    4, 0, 6, 6, 0, 2,
    4, 5, 0, 0, 5, 1,
    3, 2, 7, 7, 2, 6
};

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
    static renderer::opengl::Pipeline pipeline = [] {
        auto pipeline = renderer::opengl::Pipeline::linkShaders(
            renderer::opengl::Shader::compileFromSource(
                voxels::openForReading("/home/laurynas/Documents/Projects/Voxels/assets/shaders/basic.vert").readContent().get(),
                ShaderType::Vertex
            ),
            renderer::opengl::Shader::compileFromSource(
                voxels::openForReading("/home/laurynas/Documents/Projects/Voxels/assets/shaders/basic.frag").readContent().get(),
                ShaderType::Fragment
            )
        );
        return pipeline;
    }();
    static renderer::opengl::VertexArray vertexArray = [] {
        auto vertexArray = renderer::opengl::VertexArray({
            { GL_FLOAT, 3 },
            { GL_FLOAT, 3 },
        });

        vertexArray.copyDataIntoVertexBuffer(VERTICES, sizeof(VERTICES));
        vertexArray.copyDataIntoIndexBuffer(INDICES, sizeof(INDICES));
        return vertexArray;
    }();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);
    // glCullFace(GL_BACK);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto model = glm::mat4(1.0f);
    model = glm::scale(model, {0.5f, 0.5f, 0.5f});
    auto view = glm::lookAtRH(glm::vec3{1.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    auto [width, height] = m_surface->getViewportSize();
    auto perspective = glm::perspectiveFov(glm::radians(45.0f), static_cast<float>(width), static_cast<float>(height), 0.1f, 1024.0f);
    auto mvp = perspective * view * model;

    // mvp = glm::mat4(1.0f);

    pipeline.setUniform("mvp", mvp);
    pipeline.setUniform("brightness", 1.0f);
    pipeline.bind();
    vertexArray.bind();
    glDrawElements(GL_TRIANGLES, std::size(INDICES), GL_UNSIGNED_INT, nullptr);
    vertexArray.unBind();
    pipeline.unBind();

    m_surface->swapBuffers();
    
    m_surface->unBind();
}