//
// Created by Bellaedris on 12/12/2025.
//

#pragma once

#include <TinyObj/tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "GPU/Buffer.h"
#include "GPU/VAO.h"

#include "VectorUtils.h"

namespace mgl
{
class Mesh
{
public:
    struct VertexData
    {
        glm::vec3 pos {};
        glm::vec3 normal {};
        glm::vec2 texcoord {};

        friend bool operator<(const VertexData& lhs, const VertexData& rhs)
        {
            if(lhs.pos.x == rhs.pos.x)
            {
                if (lhs.pos.y == rhs.pos.y)
                {
                    if (lhs.pos.z == rhs.pos.z)
                        return false;
                    else
                        return lhs.pos.z < rhs.pos.z;
                }
                else
                {
                    return lhs.pos.y < rhs.pos.y;
                }
            }
            else
            {
                return lhs.pos.x < rhs.pos.x;
            }
        };
    };

private:
    #pragma region Members
    // TODO read vertexData, write these data into separate per-component vectors, then rebuild a vertexData array when
    // these data change. This will allow a better flexibility and also allow us to efficiently build our buffer when
    // read objects have different input datas (for instance, no normals or UVs)
    std::vector<VertexData> m_verticesData;
    std::vector<uint32_t> m_indices;

    gpu::Buffer m_buffer, m_indexBuffer;
    gpu::Vao m_vao;
    #pragma endregion Members
public:
    Mesh(const char* filename);

    void RecalculateNormals();
    void SetupGPU();

    void Draw() const;
    void DrawUnindexed();
};
} // mgl