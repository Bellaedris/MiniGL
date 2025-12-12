//
// Created by Bellaedris on 12/12/2025.
//

#include "App.h"
#include "MiniGL/GPU/Shader.h"

namespace mgl
{
    App::App(int width, int height, int majorVersionHint/* = 4 */, int minorVersionHint/* = 6 */)
            : m_window(width, height, majorVersionHint, minorVersionHint)
    {

    }

    void App::Run()
    {
        Init();

        gpu::Shader s;
        s.AddShaderFromFile(mgl::gpu::Shader::VERTEX, "shaders/default.vert");
        s.AddShaderFromFile(mgl::gpu::Shader::FRAGMENT, "shaders/default.frag");
        s.Create();

        uint32_t buffer, vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        while(m_window.ShouldClose() == false)
        {
            PreRender();

            Render();

            s.Bind();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            m_window.SwapBuffers();
            m_window.PollEvents();
        }
    }

    void App::Init()
    {
        // initialize what needs to be rendered here
    }

    void App::PreRender()
    {
        // If there are things you want to do before rendering ...
    }

    void App::Render()
    {
        // Render the frame

    }
}