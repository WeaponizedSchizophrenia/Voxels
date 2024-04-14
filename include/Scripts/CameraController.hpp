#pragma once

#include "CameraDescriptor.hpp"
#include "Observer.hpp"
#include "Scripts/IScript.hpp"
#include "Window/EventParser.hpp"
#include "World/Camera.hpp"
#include <memory>

namespace scripts {
    class CameraController: public IScript {
    public:
        explicit CameraController(world::Camera&& camera, const std::shared_ptr<wnd::EventParser>& eventParser)
            : m_camera(camera)
            , m_eventParser(eventParser)
            , m_mouseMoveObserver(m_eventParser->MouseMoved,
                [this](int32 x, int32 y) {
                    if(m_eventParser->isHeld(wnd::MouseButtonEvent::Button::Middle)) {
                        m_camera.yaw += static_cast<float32>(x) * sensitivity;
                        m_camera.pitch += static_cast<float32>(y) * -sensitivity;
                    }
            })
            , m_scrollObserver(m_eventParser->CursorScrolled,
                [this](const wnd::ScrollEvent& event) {
                    m_camera.fov += static_cast<float32>(event.getDelta()) * -fovIncrement;
            }) {  }

        virtual void onUpdate(float64 deltaTime) override {
            auto deltaPos = glm::vec3{
                m_eventParser->isHeld(wnd::Key::KeyD)
                ? -1.0f 
                : m_eventParser->isHeld(wnd::Key::KeyA) 
                    ? 1.0f : 0.0f,
                m_eventParser->isHeld(wnd::Key::KeyE)
                ? -1.0f 
                : m_eventParser->isHeld(wnd::Key::KeyQ) 
                    ? 1.0f : 0.0f,
                m_eventParser->isHeld(wnd::Key::KeyS)
                ? -1.0f
                : m_eventParser->isHeld(wnd::Key::KeyW)
                    ? 1.0f : 0.0f
            } * speed * static_cast<float32>(deltaTime);

            m_camera.position += m_camera.constructViewDirectionMatrix() * deltaPos;
        }

        [[nodiscard]] inline voxels::CameraDescriptor getCameraDescriptor() const noexcept {
            return m_camera.getDescriptor();
        }

        float32 sensitivity = 0.007f;
        float32 fovIncrement = 0.01f;
        float32 speed = 5.0f;

    private:
        world::Camera m_camera;
        std::shared_ptr<wnd::EventParser> m_eventParser;
        voxels::Observer<int32, int32> m_mouseMoveObserver; //< Observer for the mouse move event.
        voxels::Observer<const wnd::ScrollEvent&> m_scrollObserver; //< Observer for the scroll event.
    };
}