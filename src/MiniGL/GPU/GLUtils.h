//
// Created by Bellaedris on 12/12/2025.
//

#pragma once

#include <glad/glad.h>

namespace mgl::gpu
{
    /**
     * \brief Contains any enum/constant not linked to a single type of OpenGL object, and the associated functions.
     */
    class GLUtils
    {
    public:
        /**
         * \brief Wrapper on the OpenGL types. Functions like VertexAttribPointer will ask for a type, but it is cumbersome
         * to open Khronos' documentation to look for the correct type, while this enum should be more accessible.
         */
        enum DataType
        {
            Byte,
            UnsignedByte,
            Short,
            UnsignedShort,
            Int,
            UnsignedInt,
            Float,
            Double
        };

        static GLuint GetDataType(DataType type)
        {
            switch(type)
            {
                case DataType::Float:
                    return GL_FLOAT;
                case DataType::Byte:
                    return GL_BYTE;
                case DataType::UnsignedByte:
                    return GL_UNSIGNED_BYTE;
                case DataType::Short:
                    return GL_SHORT;
                case DataType::UnsignedShort:
                    return GL_UNSIGNED_SHORT;
                case DataType::Int:
                    return GL_INT;
                case DataType::UnsignedInt:
                    return GL_UNSIGNED_INT;
                case DataType::Double:
                    return GL_DOUBLE;
            }
        }
    };
}