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
    Texture m_colorTexture;
    Texture m_depthTexture;
public:
    Framebuffer(uint32_t width, uint32_t height);

    [[nodiscard]] uint32_t Handle() { return m_handle; };
    void Attach(Attachment attachment);
    void Bind(Type type);
    void Unbind(Type type);
};
}