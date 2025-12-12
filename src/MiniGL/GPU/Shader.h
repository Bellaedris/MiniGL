//
// Created by Bellaedris on 12/12/2025.
//

#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>

namespace mgl::gpu
{
class Shader
{
public:
    #pragma region Enum
    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        COMPUTE
    };
    #pragma endregion Enum

private:
    #pragma region Members
    uint32_t m_program;
    #pragma endregion Members

    GLuint GetShaderType(ShaderType type);
public:
    /**
     * \brief Creates an OpenGL program
     */
    Shader();

    #pragma region Methods
    /**
     * \brief Add a shader to the shader program
     * \param type The kind of shader to add. Only supports Vertex, Fragment and Compute for the moment
     * \param path Path to the shader. Remember that by default, the runtime's root is where the exe is located, so PATH/TO/PROJECT/bin
     */
    void AddShaderFromFile(ShaderType type, const char* path);

    /**
     * \brief Finalizes the creation of the shader after adding all the individual shaders we wanted.
     */
    void Create();

    /**
     * \brief Bind the shader for use
     */
    void Bind();
    #pragma endregion Methods
};
} // mgl