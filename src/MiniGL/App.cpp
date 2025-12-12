//
// Created by Bellaedris on 12/12/2025.
//

#include "App.h"
#include "MiniGL/GPU/Shader.h"
#include "MiniGL/GPU/Buffer.h"
#include "MiniGL/GPU/VAO.h"

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

        gpu::Vao vao;
        vao.Bind();
        gpu::Buffer buffer(gpu::Buffer::BufferType::Vertex);
        buffer.Bind();
        buffer.Write(sizeof(vertices), vertices, gpu::Buffer::BufferUsage::STATIC_DRAW);

        gpu::Buffer indexBuffer(gpu::Buffer::BufferType::Index);
        indexBuffer.Bind();
        indexBuffer.Write(sizeof(indices), indices, gpu::Buffer::BufferUsage::STATIC_DRAW);

        vao.SetAttribute(0, gpu::GLUtils::DataType::Float, 0, 3, sizeof(glm::vec3));

        while(m_window.ShouldClose() == false)
        {
            PreRender();

            Render();

            s.Bind();
            vao.Bind();
            glDrawElements(GL_TRIANGLES, 6, gpu::GLUtils::GetDataType(gpu::GLUtils::DataType::UnsignedInt), nullptr);
            vao.Unbind();

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