/**
 * @file Renderer.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the OpenGl renderer implementation.
 */

#include "Renderer/OpenGl/Renderer.hpp" // For declarations.
#include "File.hpp"
#include "Global.hpp"
#include "Renderer/OpenGl/Buffer.hpp"
#include "Renderer/OpenGl/Pipeline.hpp"
#include "Renderer/OpenGl/Texture.hpp"
#include "Renderer/OpenGl/VertexArray.hpp"
#include "Renderer/OpenGl/VertexBufferAttributes.hpp"
#include "Renderer/OpenGl/WindowVisitor.hpp"
#include "Renderer/ShaderType.hpp"
#include "Utilities/MeshLoading.hpp"
#include "Vertex.hpp"
#include <ctime>
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
    m_surface->bind();
    static renderer::opengl::Pipeline basicPipeline = renderer::opengl::Pipeline::linkShaders(
        renderer::opengl::Shader::compileFromSource(
            voxels::openForReading("./assets/shaders/basic.vert").readContent().get(),
            ShaderType::Vertex
        ),
        renderer::opengl::Shader::compileFromSource(
            voxels::openForReading("./assets/shaders/basic.frag").readContent().get(),
            ShaderType::Fragment
        )
    );
    static renderer::opengl::Pipeline voxelPipeline = renderer::opengl::Pipeline::linkShaders(
        renderer::opengl::Shader::compileFromSource(
            voxels::openForReading("./assets/shaders/voxelMarcher.vert").readContent().get(),
            ShaderType::Vertex
        ),
        renderer::opengl::Shader::compileFromSource(
            voxels::openForReading("./assets/shaders/voxelMarcher.frag").readContent().get(),
            ShaderType::Fragment
        )
    );
    static renderer::opengl::VertexArray vertexArray = [] {
        auto attributes = renderer::opengl::VertexBufferAttributes(voxels::Vertex::getAttributes());
        auto vertexArray = renderer::opengl::VertexArray(attributes);

        auto mesh = voxels::loadMeshes("./assets/meshes/cube.obj")[0];

        vertexArray.copyDataIntoVertexBuffer(reinterpret_cast<void*>(mesh.vertices.data()), sizeof(voxels::Vertex) * mesh.vertices.size());
        vertexArray.copyDataIntoIndexBuffer(reinterpret_cast<void*>(mesh.indices.data()), sizeof(uint32) * mesh.indices.size());
        return vertexArray;
    }();
    static auto texture = [] {
        std::srand(time(nullptr));
        static constexpr auto width = 32u, height = 32u;
        std::vector<uint32> data(width * height, 0x0);
        for(auto i = 0u; i < height; ++i) {
            for(auto j = 0u; j < width; ++j) {
                data[i * width + j] = 1 | (1 << 31);
            }
            data[i * width]         = 0xFFFFFFFF;
            data[i * width + 31]    = 0xFFFFFFFF;
        }

        return Texture2dUint32::createTexture<TextureWrap::ClampToBorder, TextureFilter::Nearest>({{data.data(), width, height}});
    }();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float time = 0.0f;
    time += 0.0006f;

    auto model = glm::mat4(1.0f);
    auto view = cameraDescriptor.constructViewMatrix();
    auto [width, height] = m_surface->getViewportSize();
    auto perspective = glm::perspectiveFov(
        cameraDescriptor.fieldOfView, 
        static_cast<float>(width), 
        static_cast<float>(height), 
        0.1f, 
        1024.0f
    );
    auto mvp = perspective * view * model;
    auto projView = perspective * view;

    basicPipeline.setUniform("model", model);
    basicPipeline.setUniform("projectionView", projView);

    voxelPipeline.setUniform("model", model);
    voxelPipeline.setUniform("projectionView", projView);
    voxelPipeline.setUniform("cameraWorldPos", cameraDescriptor.position);
    voxelPipeline.setUniform("uTexture", 0);

    
    vertexArray.bind();

    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    voxelPipeline.bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    basicPipeline.bind();

    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    vertexArray.unBind();
    basicPipeline.unBind();

    m_surface->swapBuffers();
    
    m_surface->unBind();
}