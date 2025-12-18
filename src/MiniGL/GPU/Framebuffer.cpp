//
// Created by Bellaedris on 16/12/2025.
//

#include "Framebuffer.h"

namespace mgl::gpu
{
    Framebuffer::Framebuffer(uint32_t width, uint32_t height)
        : m_width(width)
        , m_height(height)
    {
        glGenFramebuffers(1, &m_handle);
    }

    void Framebuffer::Attach(Attachment attachment)
    {
        Bind(ReadWrite);

        if (attachment == Depth)
        {
            // depth attachment
            m_depthTexture.SetSize(m_width, m_height);
            m_depthTexture.Allocate(Texture::PixelFormat::DepthComponent, GLUtils::DataType::Float);
            glNamedFramebufferTexture(m_handle, GL_DEPTH_ATTACHMENT, m_depthTexture.Handle(), 0);
        }
        else
        {
            // color attachment
            m_colorTexture.SetSize(m_width, m_height);
            m_colorTexture.Allocate(Texture::PixelFormat::RGB, GLUtils::DataType::UnsignedByte);
            glNamedFramebufferTexture(m_handle, GL_COLOR_ATTACHMENT0, m_colorTexture.Handle(), 0);
        }
        Unbind(ReadWrite);
    }

    void Framebuffer::Bind(Type type)
    {
        glViewport(0, 0, static_cast<int>(m_width), static_cast<int>(m_height));
        glBindFramebuffer(GetType(type), m_handle);
    }

    void Framebuffer::Unbind(Type type)
    {
        glBindFramebuffer(GetType(type), 0);
    }

    GLint Framebuffer::GetType(Type type)
    {
        switch (type)
        {
            case Read:
                return GL_READ_FRAMEBUFFER;
            case Write:
                return GL_DRAW_FRAMEBUFFER;
            case ReadWrite:
                return GL_FRAMEBUFFER;
            default:
                std::cerr << "Invalid Framebuffer type\n";
                return GL_FALSE;
        }
    }
}
