/**
 * @file Renderer.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the OpenGl renderer implementation.
 */

#include "Renderer/OpenGl/Renderer.hpp" // For declarations.
#include "File.hpp"
#include "Renderer/OpenGl/Buffer.hpp"
#include "Renderer/OpenGl/Pipeline.hpp"
#include "Renderer/OpenGl/VertexArray.hpp"
#include "Renderer/OpenGl/VertexBufferAttributes.hpp"
#include "Renderer/OpenGl/WindowVisitor.hpp"
#include "Renderer/ShaderType.hpp"
#include "Utilities/MeshLoading.hpp"
#include "Vertex.hpp"
#include <iostream>

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

void renderer::opengl::Renderer::render(const voxels::CameraDescriptor& cameraDescriptor) {
    static float time = 0.0f;
    m_surface->bind();
    static renderer::opengl::Pipeline pipeline = [] {
        auto pipeline = renderer::opengl::Pipeline::linkShaders(
            renderer::opengl::Shader::compileFromSource(
                voxels::openForReading("./assets/shaders/basic.vert").readContent().get(),
                ShaderType::Vertex
            ),
            renderer::opengl::Shader::compileFromSource(
                voxels::openForReading("./assets/shaders/basic.frag").readContent().get(),
                ShaderType::Fragment
            )
        );
        return pipeline;
    }();
    static renderer::opengl::VertexArray vertexArray = [] {
        auto attributes = renderer::opengl::VertexBufferAttributes(voxels::Vertex::getAttributes());
        auto vertexArray = renderer::opengl::VertexArray(attributes);

        auto mesh = voxels::loadMeshes("./assets/meshes/cube.obj")[0];

        vertexArray.copyDataIntoVertexBuffer(reinterpret_cast<void*>(mesh.vertices.data()), sizeof(voxels::Vertex) * mesh.vertices.size());
        vertexArray.copyDataIntoIndexBuffer(reinterpret_cast<void*>(mesh.indices.data()), sizeof(uint32) * mesh.indices.size());
        return vertexArray;
    }();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);
    // glCullFace(GL_BACK);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    time += 0.0006f;

    auto model = glm::mat4(1.0f);
    model = glm::scale(model, {0.5f, 0.5f, 0.5f});
    model = glm::rotate(model, time, {0.0f, 1.0f, 0.0f});
    model = glm::rotate(model, time, {1.0f, 0.0f, 0.0f});
    model = glm::rotate(model, time, {0.0f, 0.0f, 1.0f});
    auto view = glm::lookAtRH(glm::vec3{1.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    view = cameraDescriptor.constructViewMatrix();
    auto [width, height] = m_surface->getViewportSize();
    auto perspective = glm::perspectiveFov(
        cameraDescriptor.fieldOfView, 
        static_cast<float>(width), 
        static_cast<float>(height), 
        0.1f, 
        1024.0f
    );
    auto mvp = perspective * view * model;

    // mvp = glm::mat4(1.0f);

    pipeline.setUniform("mvp", mvp);
    pipeline.setUniform("brightness", 1.0f);
    pipeline.bind();
    vertexArray.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    vertexArray.unBind();
    pipeline.unBind();

    m_surface->swapBuffers();
    
    m_surface->unBind();
}