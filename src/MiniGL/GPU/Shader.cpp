//
// Created by Bellaedris on 12/12/2025.
//

#include <iostream>
#include "Shader.h"
#include "../Utils/FileUtils.h"

namespace mgl::gpu
{
    Shader::Shader()
        : m_program(glCreateProgram())
    {}

    Shader::~Shader()
    {
        glDeleteProgram(m_program);
    }

    void Shader::AddShaderFromFile(Shader::ShaderType type, const char *path)
    {
        std::optional<std::string> shaderData = utils::FileUtils::read_file(path);
        if(shaderData.has_value() == false)
        {
            std::cerr << "Couldn't open file " << path << "\n";
            return;
        }

        const char* shaderSource = shaderData->c_str();
        unsigned int shader = glCreateShader(GetShaderType(type));
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);

        // check for compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "error compiling shaders at " << path << ": " << infoLog << std::endl;
        }

        glAttachShader(m_program, shader);

        glDeleteShader(shader);
    }

    void Shader::Create()
    {
        glLinkProgram(m_program);
    }

    void Shader::Bind()
    {
        glUseProgram(m_program);
    }

    GLuint Shader::GetShaderType(Shader::ShaderType type)
    {
        switch(type)
        {
            case VERTEX:
                return GL_VERTEX_SHADER;
            case FRAGMENT:
                return GL_FRAGMENT_SHADER;
            case COMPUTE:
                return GL_COMPUTE_SHADER;
            default:
                std::cerr << "Invalid shader type";
                return GL_FALSE;
        }
    }
} // mgl