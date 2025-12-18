//
// Created by Bellaedris on 12/12/2025.
//

#include "App.h"
#include "MiniGLConfig.h"
#include "GPU/Framebuffer.h"

namespace mgl
{
    App::App(int width, int height, int majorVersionHint/* = 4 */, int minorVersionHint/* = 6 */)
            : m_window(width, height, majorVersionHint, minorVersionHint)
    {
        vendor = glGetString(GL_VENDOR);
        gpu = glGetString(GL_RENDERER);
        api = glGetString(GL_VERSION);
    }

    void App::Run()
    {
        Init();
        InitUI();

        while(m_window.ShouldClose() == false)
        {
            PreRender();

            Render();
            RenderUI();

            m_window.SwapBuffers();
            m_window.PollEvents();
        }

        Cleanup();
    }

    void App::Init()
    {
        // initialize what needs to be rendered here
        // camera
        m_camera = std::make_unique<Camera>(glm::vec3(0, 0, 0), m_window.AspectRatio(), 70.f, .01f, 1000.f);

        f = std::make_unique<gpu::Framebuffer>(m_window.Width(), m_window.Height());
        f->Attach(gpu::Framebuffer::Attachment::Color);
        f->Attach(gpu::Framebuffer::Attachment::Depth);
        f->Unbind(gpu::Framebuffer::Type::ReadWrite);

        // simple shader
        s.AddShaderFromFile(gpu::Shader::Vertex, "shaders/default.vert");
        s.AddShaderFromFile(gpu::Shader::Fragment, "shaders/default.frag");
        s.Create();

        compute.AddShaderFromFile(gpu::Shader::Compute, "shaders/tonemap.comp");
        compute.Create();

        // meshes to draw
        m_meshes.push_back(std::move(Mesh("resources/models/backpack.obj")));
        m_meshes.push_back(std::move(Mesh::GeneratePlane(1.f)));
        t = std::make_unique<gpu::Texture>(gpu::Texture::TextureTarget::Target2D, "resources/models/diffuse.jpg", true);
        m_tonemappingTexture = std::make_unique<gpu::Texture>(gpu::Texture::TextureTarget::Target2D);
        m_tonemappingTexture->SetSize(m_window.Width(), m_window.Height());
        m_tonemappingTexture->SetMinFilter(gpu::Texture::Linear);
        m_tonemappingTexture->SetMagFilter(gpu::Texture::Linear);
        m_tonemappingTexture->Allocate(gpu::Texture::RGBA, gpu::GLUtils::UnsignedByte);

        gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
        gpu::GLUtils::SetDepthTesting(true);

    }

    void App::PreRender()
    {
        m_deltaTime = m_deltaTimer.Elapsed();
        // If there are things you want to do before rendering ...
        glm::vec3 inputDir = m_window.GatherInput();
        m_camera->ProcessMouseMovement(m_window.GetMousePosOffset());
        m_camera->UpdatePosition(inputDir, m_deltaTime);

        // update dt
        m_deltaTimer.Reset();
    }

    void App::Render()
    {
        m_gpuDeltaTime.Begin();
        // clear the framebuffer
        gpu::GLUtils::Clear();

        // Render the frame
        f->Bind(gpu::Framebuffer::Type::Write);
        gpu::GLUtils::SetDepthTesting(true);
        gpu::GLUtils::Clear();

        s.Bind();
        s.UniformData("viewMatrix", m_camera->View());
        s.UniformData("projectionMatrix", m_camera->Projection());
        t->Bind(0);
        s.UniformData("albedo", 0);
        for(const auto& mesh : m_meshes)
            mesh.Draw();

        // post processing
        compute.Bind();
        m_tonemappingTexture->BindImage(0, 0, gpu::GLUtils::Write);
        f->ColorTexture()->Bind(1);
        compute.UniformData("framebuffer", 1);

        compute.Dispatch(1360, 768, 1);
        glMemoryBarrier( GL_ALL_BARRIER_BITS );

        f->Unbind(gpu::Framebuffer::Type::ReadWrite);
        glViewport(0, 0, 1360, 768);

        glBlitNamedFramebuffer(f->Handle(), 0, 0, 0, 1360, 768, 0, 0, 1360, 768,  GL_COLOR_BUFFER_BIT, GL_NEAREST);
        m_gpuDeltaTime.Begin();
    }

    void App::RenderUI()
    {
        // Initialise a new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // the actual UI goes here
        ImGui::SetNextWindowPos({0, 0}, 0, {0, 0});
        ImGui::Begin(
                "Infos",
                nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize
        );
        {
            ImGui::Text("MiniGL version %d.%d", MiniGL_VERSION_MAJOR, MiniGL_VERSION_MINOR);
            ImGui::Text("OpenGL%s", api);
            ImGui::Text("%s %s", vendor, gpu);

            float average = 0.0f;
            for (float n: framerate)
                average += n;
            average /= (float) IM_ARRAYSIZE(framerate);
            ImGui::Text("Average %02fFPS", average);
            uint64_t elapsedGPU = m_gpuDeltaTime.Elapsed();
            // ugly way to convert to a readable format but I'm kinda lazy and don't want to fight both
            // std::chrono types and c strings
            float cpuSeconds = m_deltaTime * 1000.f;
            int milli = (int) (elapsedGPU / 1000000);
            int micro = (int) ((elapsedGPU / 1000) % 1000);
            ImGui::Text(
                    "cpu %03fms\ngpu %02dms % 03dus",
                    cpuSeconds,
                    milli, micro
            );
            ImGui::End();
        }

        // render the UI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void App::Cleanup()
    {
        // cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void App::InitUI()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        ImGui_ImplGlfw_InitForOpenGL(m_window.GetWindow(), true);
        ImGui_ImplOpenGL3_Init();
    }
}
