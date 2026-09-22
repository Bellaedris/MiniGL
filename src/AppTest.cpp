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

        // create a framebuffer and its attachments
        f = std::make_unique<mgl::gpu::Framebuffer>(m_window.Width(), m_window.Height());
        mgl::gpu::Texture::TextureDesc colorDesc =
        {
            .target = mgl::gpu::Texture::TextureTarget::Target2D,
            .width = m_window.Width(),
            .height = m_window.Height(),
            .format = mgl::gpu::Texture::PixelFormat::RGBA,
            .dataType = mgl::gpu::GLUtils::DataType::Float,
            .minFilter = mgl::gpu::Texture::Filtering::LinearMipMapLinear,
            .magFilter = mgl::gpu::Texture::Filtering::Linear,
            .wrapMode = mgl::gpu::Texture::WrapMode::ClampToEdge
        };
        m_colorTexture = std::make_shared<mgl::gpu::Texture>(colorDesc);

        mgl::gpu::Texture::TextureDesc depthDesc =
        {
            .target = mgl::gpu::Texture::TextureTarget::Target2D,
            .width = m_window.Width(),
            .height = m_window.Height(),
            .format = mgl::gpu::Texture::PixelFormat::DepthComponent,
            .dataType = mgl::gpu::GLUtils::DataType::Float,
            .minFilter = mgl::gpu::Texture::Filtering::Nearest,
            .magFilter = mgl::gpu::Texture::Filtering::Nearest,
            .wrapMode = mgl::gpu::Texture::WrapMode::ClampToBorder
        };
        m_depthTexture = std::make_shared<mgl::gpu::Texture>(depthDesc);

        f->Attach(mgl::gpu::Framebuffer::Attachment::Color, m_colorTexture);
        f->Attach(mgl::gpu::Framebuffer::Attachment::Depth, m_depthTexture);
        f->Unbind(mgl::gpu::Framebuffer::Type::ReadWrite);

        // simple shader
        s.AddShaderFromFile(mgl::gpu::Shader::ShaderSource(mgl::gpu::Shader::Vertex, "shaders/default.vert"));
        s.AddShaderFromFile(mgl::gpu::Shader::ShaderSource(mgl::gpu::Shader::Fragment, "shaders/default.frag"));
        s.Create();

        compute.AddShaderFromFile(mgl::gpu::Shader::ShaderSource(mgl::gpu::Shader::Compute, "shaders/tonemap.comp"));
        compute.Create();

        // meshes to draw
        m_meshes.push_back(std::move(mgl::Mesh("resources/models/backpack.obj")));
        m_meshes.push_back(std::move(mgl::Mesh::GeneratePlane(1.f)));
        t = std::make_unique<mgl::gpu::Texture>(mgl::gpu::Texture::TextureTarget::Target2D, "resources/models/diffuse.jpg", true);

        mgl::gpu::Texture::TextureDesc tonemapDesc =
        {
            .target = mgl::gpu::Texture::TextureTarget::Target2D,
            .width = m_window.Width(),
            .height = m_window.Height(),
            .format = mgl::gpu::Texture::PixelFormat::RGBA,
            .dataType = mgl::gpu::GLUtils::DataType::UnsignedByte,
            .minFilter = mgl::gpu::Texture::Filtering::LinearMipMapLinear,
            .magFilter = mgl::gpu::Texture::Filtering::Linear,
            .wrapMode = mgl::gpu::Texture::WrapMode::ClampToBorder
        };
        m_tonemappingTexture = std::make_unique<mgl::gpu::Texture>(tonemapDesc);

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
    std::shared_ptr<mgl::gpu::Texture> m_colorTexture;
    std::shared_ptr<mgl::gpu::Texture> m_depthTexture;
    std::unique_ptr<mgl::gpu::Texture> m_tonemappingTexture;
    std::unique_ptr<mgl::gpu::Framebuffer> f;
};

int main()
{
    AppTest app(1360, 768);
    app.Run();
}
