//
// Created by Bellaedris on 16/12/2025.
//

#pragma once

#include <cstdint>
#include <memory>

#include "Texture.h"

namespace mgl::gpu
{
/**
 * By default, any draw command will draw on a default framebuffer, an object that holds the textures wil draw into (colors, depth and stencil).
 * It is possible to create our own framebuffers. It is very useful, for instance we might want to draw our scene in a framebuffer,
 * then apply post-processing in another drawing pass, using the result of our scene drawing ass a simple texture!
 */
class Framebuffer
{
public:
    #pragma region Enums
        /**
         * We can bind a framebuffer of different type
         */
        enum Type
        {
            Read,
            Write,
            ReadWrite
        };

        /**
         * a framebuffer can have different textures attached: 0 to 8 color textures, that we can write to using a fragment shader,
         * a depth texture, that stores depth when we draw, and a stencil texture, that allows us to assign values to pixels we can use as masks later on
         */
        enum Attachment
        {
            Color   = 1 << 0,
            Depth   = 1 << 1,
            Stencil = 1 << 2
        };

        static GLint GetType(Type type);
        static GLint GetAttachment(Attachment attachment);
    #pragma endregion
private:
    #pragma region Members
    uint32_t m_handle;

    uint32_t m_width;
    uint32_t m_height;
    /** \brief a framebuffer can hold up to 8 color attachments */
    std::array<bool, 8> m_attachmentsState {};
    std::vector<uint32_t> m_usedAttachments {};
    #pragma endregion Members
public:
    ~Framebuffer();
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer& other) = delete;
    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    Framebuffer(uint32_t width, uint32_t height);

    void SetSize(uint32_t width, uint32_t height) { m_width = width; m_height = height; };
    [[nodiscard]] uint32_t Handle() const { return m_handle; };

    /**
     * \brief Attach a texture to a framebuffer. Can be used to add depth, stencil or multiple color attachments
     * \param attachment The type of attachment to bind to this framebuffer
     * \param texture the texture to attach
     * \param colorAttachmentId Optional. Only useful for color attachment, tell which color attachment will be bound,
     * more on that here https://registry.khronos.org/OpenGL-Refpages/gl4/html/glFramebufferTexture.xhtml
     */
    void Attach(Attachment attachment, const std::shared_ptr<Texture>& texture, int colorAttachmentId = 0);
    void Bind(Type type) const;
    void Unbind(Type type) const;

    #pragma region StaticMethods
        /**
         * Copies a given portion of a framebuffer into another framebuffer
         * @param src framebuffer that will be copied
         * @param dst framebuffer that will receive
         * @param srcX0 source lower bounds on the X axis
         * @param srcY0 source lower bounds on the Y axis
         * @param srcX1 source upper bounds on the X axis
         * @param srcY1 source upper bounds on the Y axis
         * @param dstX0 destination lower bounds on the X axis
         * @param dstY0 destination lower bounds on the Y axis
         * @param dstX1 destination upper bounds on the X axis
         * @param dstY1 destination upper bounds on the Y axis
         * @param whatToCopy what attachment should be copied for each buffer
         * @param filter how to filter the data, for instance if we copy a 50x50 framebuffer into a 100x100 framebuffer, what method should we use to interpolate
         */
        static void BlitFramebuffer(const std::unique_ptr<Framebuffer>& src, const std::unique_ptr<Framebuffer>& dst, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, Attachment whatToCopy, Texture::Filtering filter);
    #pragma endregion StaticMethods
};
}