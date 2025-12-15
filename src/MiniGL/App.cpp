//
// Created by Bellaedris on 12/12/2025.
//

#include "App.h"

namespace mgl
{
    App::App(int width, int height, int majorVersionHint/* = 4 */, int minorVersionHint/* = 6 */)
            : m_window(width, height, majorVersionHint, minorVersionHint)
    {

    }

    void App::Run()
    {
        Init();

        while(m_window.ShouldClose() == false)
        {
            PreRender();

            Render();

            m_window.SwapBuffers();
            m_window.PollEvents();
        }
    }

    void App::Init()
    {
        // initialize what needs to be rendered here
        // camera
        m_camera = std::make_unique<Camera>(glm::vec3(0, 0, 0), 16.f / 9.f, 70.f, .01f, 1000.f);

        // simple shader
        s.AddShaderFromFile(mgl::gpu::Shader::VERTEX, "shaders/default.vert");
        s.AddShaderFromFile(mgl::gpu::Shader::FRAGMENT, "shaders/default.frag");
        s.Create();

        // meshes to draw
        m_meshes.emplace_back("resources/models/backpack.obj");

        gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
        gpu::GLUtils::SetDepthTesting(true);
    }

    void App::PreRender()
    {
        // If there are things you want to do before rendering ...
        glm::vec3 inputDir = m_window.GatherInput();
        m_camera->ProcessMouseMovement(m_window.GetMousePosOffset());
        m_camera->UpdatePosition(inputDir, m_deltaTimer.Elapsed());

        // update dt
        m_deltaTimer.Reset();
    }

    void App::Render()
    {
        // clear the framebuffer
        gpu::GLUtils::Clear();

        // Render the frame
        s.Bind();
        s.UniformData("viewMatrix", m_camera->View());
        s.UniformData("projectionMatrix", m_camera->Projection());
        for(const auto& mesh : m_meshes)
            mesh.Draw();
    }
}