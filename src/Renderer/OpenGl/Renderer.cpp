/**
 * @file Renderer.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the OpenGl renderer implementation.
 */

#include "Renderer/OpenGl/Renderer.hpp" // For declarations.
#include "Exception.hpp" // For exceptions.
#include "Renderer/OpenGl/WindowVisitor.hpp" // For the visitor.
#include "glad/glad.h" // For glad functions.

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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_surface->swapBuffers();
    
    m_surface->unBind();
}