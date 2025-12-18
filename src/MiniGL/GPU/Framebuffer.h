//
// Created by Bellaedris on 16/12/2025.
//

#pragma once

#include <cstdint>

#include "Texture.h"

namespace mgl::gpu
{
class Framebuffer
{
public:
    #pragma region Enums
        enum Type
        {
            Read,
            Write,
            ReadWrite
        };

        enum Attachment
        {
            Color,
            Depth
        };

        static GLint GetType(Type type);
    #pragma endregion
private:
    uint32_t m_handle;

    uint32_t m_width;
    uint32_t m_height;
    std::unique_ptr<Texture> m_colorTexture {nullptr};
    std::unique_ptr<Texture> m_depthTexture {nullptr};
public:
    ~Framebuffer();
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer& other) = delete;
    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    Framebuffer(uint32_t width, uint32_t height);

    [[nodiscard]] uint32_t Handle() { return m_handle; };
    [[nodiscard]] const std::unique_ptr<Texture>& ColorTexture() const;
    [[nodiscard]] const std::unique_ptr<Texture>& DepthTexture() const;
    void Attach(Attachment attachment);
    void Bind(Type type);
    void Unbind(Type type);
};
}