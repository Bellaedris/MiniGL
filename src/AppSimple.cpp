#include "MiniGL/App.h"

class AppSimple : public mgl::App
{
public:
    AppSimple(int width, int height) : App(width, height, 4, 6) {};

protected:
    void Init() override
    {
        // initialize what needs to be rendered here
        // camera
        m_camera = std::make_unique<mgl::Camera>(glm::vec3(0, 0, 0), m_window.AspectRatio(), 70.f, .01f, 1000.f);

        // simple shader
        s.AddShaderFromFile(mgl::gpu::Shader::ShaderSource(mgl::gpu::Shader::Vertex, "shaders/default.vert"));
        s.AddShaderFromFile(mgl::gpu::Shader::ShaderSource(mgl::gpu::Shader::Fragment, "shaders/default.frag"));
        s.Create();

        // meshes to draw
        m_meshes.push_back(std::move(mgl::Mesh("resources/models/backpack.obj")));
        t = std::make_unique<mgl::gpu::Texture>(mgl::gpu::Texture::TextureTarget::Target2D, "resources/models/diffuse.jpg", true);

        mgl::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
        mgl::gpu::GLUtils::SetDepthTesting(true);
        mgl::gpu::GLUtils::SetViewportSize(m_window.Width(), m_window.Height());
    }

    void Render() override
    {
        m_gpuDeltaTime.Begin();
        // clear the framebuffer
        mgl::gpu::GLUtils::Clear();

        // Render the frame
        mgl::gpu::GLUtils::Clear();

        s.Bind();
        s.UniformData("viewMatrix", m_camera->View());
        s.UniformData("projectionMatrix", m_camera->Projection());
        t->Bind(0);
        for(const auto& mesh : m_meshes)
            mesh.Draw();

        m_gpuDeltaTime.End();
    }

private:
    mgl::gpu::Shader s;
    std::unique_ptr<mgl::gpu::Texture> t;
};


