#include "instancerendering.h"

InstanceRendering::InstanceRendering()
{

}

//bool InitFromScene(const aiScene* pScene, const std::string& Filename)
//{
//    // Generate and populate the buffers with vertex attributes and the indices
//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0],
//                GL_STATIC_DRAW);
//    glEnableVertexAttribArray(POSITION_LOCATION);
//    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0],
//                GL_STATIC_DRAW);
//    glEnableVertexAttribArray(TEX_COORD_LOCATION);
//    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0],
//                GL_STATIC_DRAW);
//    glEnableVertexAttribArray(NORMAL_LOCATION);
//    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0],
//                GL_STATIC_DRAW);

//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);

//    for (unsigned int i = 0; i < 4 ; i++) {
//        glEnableVertexAttribArray(WVP_LOCATION + i);
//        glVertexAttribPointer(WVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f),
//                                (const GLvoid*)(sizeof(GLfloat) * i * 4));
//        glVertexAttribDivisor(WVP_LOCATION + i, 1);
//    }

//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);

//    for (unsigned int i = 0; i < 4 ; i++) {
//        glEnableVertexAttribArray(WORLD_LOCATION + i);
//        glVertexAttribPointer(WORLD_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f),
//                                (const GLvoid*)(sizeof(GLfloat) * i * 4));
//        glVertexAttribDivisor(WORLD_LOCATION + i, 1);
//    }

//    return GLCheckError();
//}

//void Render(unsigned int NumInstances, const gsl::Matrix4x4* WVPMats, const gsl::Matrix4x4* WorldMats)
//{
//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

//    glBindVertexArray(m_VAO);

//    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
//        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

//        assert(MaterialIndex < m_Textures.size());

//        if (m_Textures[MaterialIndex]) {
//            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
//        }

//        glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
//                                         m_Entries[i].NumIndices,
//                                         GL_UNSIGNED_INT,
//                                         (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
//                                         NumInstances,
//                                         m_Entries[i].BaseVertex);
//    }

//    // Make sure the VAO is not changed from the outside
//    glBindVertexArray(0);
//}
