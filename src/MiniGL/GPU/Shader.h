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
    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        COMPUTE
    };
    #pragma endregion InnerStructs

private:
    #pragma region Members
    uint32_t m_program;
    std::vector<std::string> m_shadersList;
    #pragma endregion Members

    GLuint GetShaderType(ShaderType type);
    #pragma region InnerStructs

public:
    Shader();

    void AddShaderFromFile(ShaderType type, const char* file);
    void Create();
    void Bind();
};
} // mgl