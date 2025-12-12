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

    void Vao::Bind()
    {
        glBindVertexArray(m_vao);
    }

    void Vao::SetAttribute(const Buffer& buffer, uint32_t location, GLUtils::DataType type, uint32_t offset, uint32_t tupleSize, uint32_t stride)
    {
        buffer.Bind();
        Bind();
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, tupleSize, GLUtils::GetDataType(type), GL_FALSE, stride, (void*)offset);
    }
} // mgl::gpu