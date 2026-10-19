//
// Created by Bellaedris on 16/12/2025.
//

#pragma once

#include <cstdint>
#include <glad/glad.h>

#include <stb/stb_image.h>

#include "GLUtils.h"

namespace mgl::gpu
{
/**
 * \brief Wraps OpenGL 2D textures creation
 */
class Texture
{
public:
#pragma region Enums
    /**
     * \brief Different texture targets we can bind to. We only handle 2D textures for now.
     */
    enum TextureTarget
    {
        Target2D,
        TargetCubemap
    };

    /**
     * \brief How to handle UVs overflow. See https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml,
     * section "GL_TEXTURE_WRAP_XXX"
     */
    enum WrapMode
    {
        Repeat,
        RepeatMirror,
        ClampToEdge,
        ClampToBorder
    };

    /**
     * How to filter the pixels when sampling the texture.
     */
    enum Filtering
    {
        Nearest,
        Linear,
        NearestMipMapNearest,
        NearestMipMapLinear,
        LinearMipMapNearest,
        LinearMipMapLinear
    };

    /**
     * \brief What kind of data this texture holds
     */
    enum PixelFormat
    {
        DepthComponent,
        DepthStencil,
        Red,
        RG,
        RGB,
        RGBA,
        SRGB
    };
#pragma endregion Enums

#pragma region EnumAccessFunctions
    static GLint GetTextureTarget(TextureTarget target);
    static GLint GetWrapMode(WrapMode mode);
    static GLint GetFiltering(Filtering filter);
    // TODO this function is horrible. Replace by a proper gigantic enum with all the formats we will need.
    static GLint GetPixelInternalFormat(PixelFormat format, GLUtils::DataType type);
    static GLint GetPixelFormat(PixelFormat format);
    static GLint GetImageFormat(PixelFormat format);
#pragma endregion EnumAccessFunctions
#pragma region Structs

    /**
     * \brief The parameters required to build a texture. it would be very ugly to put all these as parameters in the constructor so they're in a struct.
     */
    struct TextureDesc
    {
        TextureTarget target;
        int width;
        int height;
        PixelFormat format;
        GLUtils::DataType dataType;
        Filtering minFilter {LinearMipMapLinear};
        Filtering magFilter {Linear};
        WrapMode wrapMode {WrapMode::Repeat};
    };
#pragma endregion Structs
private:
#pragma region Members
    /** \brief relative path to the texture. Only used for textures loaded from files, textures created at runtime have a blank path */
    std::string m_path;

    uint32_t m_handle {0};

    int m_width {0};
    int m_height {0};
    TextureTarget m_target;
    WrapMode m_wrappingMethod {WrapMode::Repeat};
    Filtering m_minFilter {Filtering::LinearMipMapLinear};
    Filtering m_magFilter {Filtering::Linear};
    PixelFormat m_format {};
    GLUtils::DataType m_dataType {};
#pragma endregion Members
public:
    Texture(const TextureDesc& desc);
    Texture(TextureTarget target, const char* path, bool generateMipmaps);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    /** \brief returns the ID of this texture. */
    [[nodiscard]] inline uint32_t Handle() {return m_handle; };
    /** \brief sets the texture as active, on whatever texture unit is selected at the moment */
    void Bind();
    /** \brief sets the texture as active on a specified texture unit \param unit the texture unit we bind this texture to*/
    void Bind(uint32_t unit);
    /**
     * \brief Sometimes, we don't need the whole texture with its mipmaps. This function will bind a single mip level of the texture to the specified texture unit.
     * The object we map won't be a sampler2D but an image2D. This is especially useful for compute shaders, since we can't write to a sampler2D, we need an image. see the tonemap.comp shader for an example.
     * @param unit texture unit to bind the mip level to
     * @param mipLevel element of the mip chain to bind. 0 is the full size texture, and each successive mip level will divide the size by 2.
     * @param access Will we only read to the image? Write to it? Both?
     */
    void BindImage(uint32_t unit, uint32_t mipLevel, GLUtils::Access access);

    /**
     * Allocate the Texture to the GPU without giving it any data. Very useful for textures that will be given to framebuffers, for instance.
     * @param format format of the texture
     * @param dataType Data type of each pixel
     */
    void Allocate(PixelFormat format, GLUtils::DataType dataType);

    /**
     * \brief Allocate storage for a number of mipmaps. This should be done on allocated textures. Once the mips have been
     * allocated, the texture will be considered final and CANNOT be resized.
     * Useful to build a mip chain by hand.
     * \param levels number of mip levels that we want to allocate. When requesting a level n, it is possible that some mips
     * will have a size < 1. In that case, the function will allocate the maximal number of integer size mips.
     */
    void AllocateMipmapsStorage(int levels);

    /**
     * \brief Allocate the memory again, using the same formats and data types, but to take into account any change
     * that could've happened in between.
     */
    void Reallocate();

    /**
     * Allocate a texture while also filling its data with an input
     * @param data pointer to pixel data. If the data sent do not match with format/datatype, the behaviour is undefined.
     * @param format format of the texture
     * @param dataType Type of the pixels
     */
    void Write(void* data, PixelFormat format, GLUtils::DataType dataType);

    void SetMinFilter(Filtering filter);
    void SetMagFilter(Filtering filter);
    void SetWrapMode(WrapMode mode);
    void SetBorderColor(const glm::vec4& color);
    void SetSize(int width, int height);

    glm::ivec2 Size() const { return {m_width, m_height}; };
    std::string Path() const { return m_path; }
};
} // mgl