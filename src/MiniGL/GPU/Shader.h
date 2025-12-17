//
// Created by Bellaedris on 12/12/2025.
//

#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

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

    #pragma region Uniform data
    GLint GetLocation(const std::string &name) const
    {
        return glGetUniformLocation(m_program, name.c_str());
    }

    void UniformData(const std::string& name, const glm::mat4 mat) const
    {
        glUniformMatrix4fv(GetLocation(name), 1, false, glm::value_ptr(mat));
    }

    void UniformData(const std::string& name, const float x, const float y, const float z, const float w) const
    {
        glUniform4f(GetLocation(name), x, y, z, w);
    }

    void UniformData(const std::string& name, const glm::vec4& data) const
    {
        glUniform4f(GetLocation(name), data.x, data.y, data.z, data.w);
    }

    void UniformData(const std::string& name, const glm::vec3& data) const
    {
        glUniform3f(GetLocation(name), data.x, data.y, data.z);
    }

    void UniformData(const std::string& name, const glm::vec2& data) const
    {
        glUniform2f(GetLocation(name), data.x, data.y);
    }

    void UniformData(const std::string &name, const float x, const float y, const float z) const
    {
        glUniform3f(GetLocation(name), x, y, z);
    }

    void UniformData(const std::string& name, const float x) const
    {
        glUniform1f(GetLocation(name), x);
    }

    void UniformData(const std::string& name, const int x) const
    {
        glUniform1i(GetLocation(name), x);
    }
    #pragma endregion Uniform data
};
} // mgl