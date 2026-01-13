#include "MiniGL/App.h"

class AppTest : public mgl::App
{
public:
    AppTest(int width, int height) : App(width, height, 4, 6) {};

    void Init() override
    {
        // initialize what needs to be rendered here
        // camera
        m_camera = std::make_unique<mgl::Camera>(glm::vec3(0, 0, 0), m_window.AspectRatio(), 70.f, .01f, 1000.f);

        f = std::make_unique<mgl::gpu::Framebuffer>(m_window.Width(), m_window.Height());
        f->Attach(mgl::gpu::Framebuffer::Attachment::Color);
        f->Attach(mgl::gpu::Framebuffer::Attachment::Depth);
        f->Unbind(mgl::gpu::Framebuffer::Type::ReadWrite);

        // simple shader
        s.AddShaderFromFile(mgl::gpu::Shader::Vertex, "shaders/default.vert");
        s.AddShaderFromFile(mgl::gpu::Shader::Fragment, "shaders/default.frag");
        s.Create();

        compute.AddShaderFromFile(mgl::gpu::Shader::Compute, "shaders/tonemap.comp");
        compute.Create();

        // meshes to draw
        m_meshes.push_back(std::move(mgl::Mesh("resources/models/backpack.obj")));
        m_meshes.push_back(std::move(mgl::Mesh::GeneratePlane(1.f)));
        t = std::make_unique<mgl::gpu::Texture>(mgl::gpu::Texture::TextureTarget::Target2D, "resources/models/diffuse.jpg", true);
        m_tonemappingTexture = std::make_unique<mgl::gpu::Texture>(mgl::gpu::Texture::TextureTarget::Target2D);
        m_tonemappingTexture->SetSize(m_window.Width(), m_window.Height());
        m_tonemappingTexture->SetMinFilter(mgl::gpu::Texture::Linear);
        m_tonemappingTexture->SetMagFilter(mgl::gpu::Texture::Linear);
        m_tonemappingTexture->Allocate(mgl::gpu::Texture::RGBA, mgl::gpu::GLUtils::UnsignedByte);

        mgl::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
        mgl::gpu::GLUtils::SetDepthTesting(true);
    }

    void Render() override
    {
        m_gpuDeltaTime.Begin();
        // clear the framebuffer
        mgl::gpu::GLUtils::Clear();

        // Render the frame
        f->Bind(mgl::gpu::Framebuffer::Type::Write);
        mgl::gpu::GLUtils::SetDepthTesting(true);
        mgl::gpu::GLUtils::Clear();

        s.Bind();
        s.UniformData("viewMatrix", m_camera->View());
        s.UniformData("projectionMatrix", m_camera->Projection());
        t->Bind(0);
        s.UniformData("albedo", 0);
        for(const auto& mesh : m_meshes)
            mesh.Draw();

        // post processing
        compute.Bind();
        m_tonemappingTexture->BindImage(0, 0, mgl::gpu::GLUtils::Write);
        f->ColorTexture()->Bind(1);
        compute.UniformData("framebuffer", 1);

        compute.Dispatch(1360, 768, 1);
        compute.Wait();

        f->Unbind(mgl::gpu::Framebuffer::Type::ReadWrite);
        mgl::gpu::GLUtils::SetViewportSize(1360, 768);

        mgl::gpu::Framebuffer::Attachment whatToCopy = mgl::gpu::Framebuffer::Attachment::Color;
        mgl::gpu::Framebuffer::BlitFramebuffer(f, nullptr, 0, 0, 1360, 768, 0, 0, 1360, 768, whatToCopy, mgl::gpu::Texture::Filtering::Linear);
        m_gpuDeltaTime.Begin();
    }

private:
    mgl::gpu::Shader s;
    mgl::gpu::Shader compute;
    std::unique_ptr<mgl::gpu::Texture> t;
    std::unique_ptr<mgl::gpu::Texture> m_tonemappingTexture;
    std::unique_ptr<mgl::gpu::Framebuffer> f;
};

int main()
{
    AppTest app(1360, 768);
    app.Run();
}
