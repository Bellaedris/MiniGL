//
// Created by Bellaedris on 16/12/2025.
//

#include "Texture.h"

namespace mgl::gpu
{
    Texture::Texture(Texture::TextureTarget target)
        : m_target(target)
    {
        glGenTextures(1, &m_handle);
    }

    Texture::Texture(Texture::TextureTarget target, const char *path, bool generateMipmaps)
        : m_target(target)
    {
        glGenTextures(1, &m_handle);

        // read image
        // flip the image since OpenGL has reverse y compared to images
        //stbi_set_flip_vertically_on_load(true);
        int width, height, numChannels;
        unsigned char* data = stbi_load(path, &width, &height, &numChannels, 0);
        if(data == nullptr)
        {
            std::cerr << "Couldn't read image at " << path << "\n";
            return;
        }

        PixelFormat format = PixelFormat::RGB;
        if(numChannels == 4)
            format = PixelFormat::RGBA;
        SetWrapMode(WrapMode::ClampToEdge);

        SetSize(width, height);
        Write(data, format, GLUtils::DataType::UnsignedByte);
        if(generateMipmaps)
            glGenerateMipmap(GetTextureTarget(m_target));

        stbi_image_free(data);
    }

    void Texture::SetMinFilter(Texture::Filtering filter)
    {
        Bind();
        m_minFilter = filter;
        glTexParameteri(GetTextureTarget(m_target), GL_TEXTURE_MIN_FILTER, GetFiltering(filter));
    }

    void Texture::SetMagFilter(Texture::Filtering filter)
    {
        Bind();
        m_magFilter = filter;
        glTexParameteri(GetTextureTarget(m_target), GL_TEXTURE_MAG_FILTER, GetFiltering(filter));
    }

    void Texture::SetWrapMode(Texture::WrapMode mode)
    {
        Bind();
        m_wrappingMethod = mode;
        glTexParameteri(GetTextureTarget(m_target), GL_TEXTURE_WRAP_S, GetWrapMode(mode));
        glTexParameteri(GetTextureTarget(m_target), GL_TEXTURE_WRAP_T, GetWrapMode(mode));
    }

    void Texture::SetSize(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    void Texture::Allocate(Texture::PixelFormat format, GLUtils::DataType dataType)
    {
        Bind();
        glTexImage2D(
            GetTextureTarget(m_target),
            0,
            GetPixelFormat(format),
            m_width,
            m_height,
            0,
            GLUtils::GetDataType(dataType),
            GetPixelFormat(format),
            nullptr
        );
    }

    void Texture::Write(void *data, Texture::PixelFormat format, GLUtils::DataType dataType)
    {
        Bind();
        glTexImage2D(
                GetTextureTarget(m_target),
                0,
                GL_RGB,
                m_width,
                m_height,
                0,
                GetPixelFormat(format),
                GLUtils::GetDataType(dataType),
                data
        );
    }

    void Texture::Bind()
    {
        glBindTexture(GetTextureTarget(m_target), m_handle);
    }

    void Texture::Bind(uint32_t unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        Bind();
    }

    #pragma region EnumAccessFunctions
    GLint Texture::GetTextureTarget(Texture::TextureTarget target)
    {
        switch(target)
        {
            case Target2D:
                return GL_TEXTURE_2D;
            case TargetCubemap:
                return GL_TEXTURE_CUBE_MAP;
            default:
                std::cerr << "Unknown Texture target";
                return GL_FALSE;
        };
    }

    GLint Texture::GetWrapMode(Texture::WrapMode mode)
    {
        switch(mode)
        {
            case Repeat:
                return GL_REPEAT;
            case RepeatMirror:
                return GL_MIRRORED_REPEAT;
            case ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case ClampToBorder:
                return GL_CLAMP_TO_BORDER;
            default:
                std::cerr << "Unknown texture wrap mode";
                return GL_FALSE;
        }
    }

    GLint Texture::GetFiltering(Texture::Filtering filter)
    {
        switch(filter)
        {
            case Nearest:
                return GL_NEAREST;
            case Linear:
                return GL_LINEAR;
            case NearestMipMapNearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            case NearestMipMapLinear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case LinearMipMapNearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case LinearMipMapLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
            default:
                std::cerr << "Unknown filtering method";
                return GL_FALSE;
        }
    }

    GLint Texture::GetPixelFormat(Texture::PixelFormat format)
    {
        switch(format)
        {
            case DepthComponent:
                return GL_DEPTH_COMPONENT;
            case DepthStencil:
                return GL_DEPTH_STENCIL;
            case Red:
                return GL_RED;
            case RG:
                return GL_RG;
            case RGB:
                return GL_RGB;
            case RGBA:
                return GL_RGBA;
            case SRGB:
                return GL_SRGB;
            default:
                std::cerr << "Unknown pixel format";
                return GL_FALSE;
        }
    }
    #pragma endregion EnumAccessFunctions
} // mgl