/**
 * @file Renderer.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the OpenGl renderer implementation.
 */

#include "Renderer/OpenGl/Renderer.hpp" // For declarations.
#include "Renderer/OpenGl/WindowVisitor.hpp" // For the visitor.
#include <GL/gl.h> // For OpenGl functions.
#include <GL/glx.h> // For glx functions.


void DrawAQuad() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.75, -0.75, 0.0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0.75, -0.75, 0.0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f( 0.0, 0.75, 0.0);
    glEnd();
}

renderer::opengl::Renderer::Renderer(const wnd::IWindow& window)
    : m_surface(window.acceptVisitor(renderer::opengl::WindowVisitor())) 
{
    
}

void renderer::opengl::Renderer::render(int32 width, int32 height) {
    m_surface->bind();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    DrawAQuad();
    m_surface->swapBuffers();
    m_surface->unBind();
}