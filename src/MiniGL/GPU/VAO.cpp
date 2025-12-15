//
// Created by Bellaedris on 12/12/2025.
//

#include "VAO.h"

namespace mgl::gpu
{
    Vao::Vao()
    {
        glGenVertexArrays(1, &m_vao);
    }

    void Vao::Bind() const
    {
        glBindVertexArray(m_vao);
    }

    void Vao::Unbind() const
    {
        glBindVertexArray(0);
    }

    void Vao::SetAttribute(int location, GLUtils::DataType type, int offset, int tupleSize, int stride)
    {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, tupleSize, GLUtils::GetDataType(type), GL_FALSE, stride, (void*)offset);
    }
} // mgl::gpu