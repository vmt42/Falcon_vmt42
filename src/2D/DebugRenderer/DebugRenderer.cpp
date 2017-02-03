#include <GL/glew.h>
#include "DebugRenderer.h"

namespace
{
    const char* VERT_SRC = R"(#version 120

attribute vec2 vertexPosition;
attribute vec4 vertexColor;

varying vec4 fragmentColor;
varying vec2 fragmentPosition;

uniform mat4 P;

void main()
{
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragmentColor = vertexColor;
    fragmentPosition = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
})";

    const char* FRAG_SRC = R"(#version 120

varying vec4 fragmentColor;
varying vec2 fragmentPosition;


void main()
{
    gl_FragColor = fragmentColor;
})";
}


namespace Falcon
{
    void DebugRenderer::init()
    {
        // Shader init
        m_shaderProgram.compileShadersFromSource(VERT_SRC, FRAG_SRC);
        m_shaderProgram.addAttribute("vertexPosition");
        m_shaderProgram.addAttribute("vertexColor");
        m_shaderProgram.linkShaders();

        // Set up buffers
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ibo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

        glBindVertexArray(0);
    }

    void DebugRenderer::end()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        // Orphan the buffer
        glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);

        //Upload the data
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_verts.size() * sizeof(DebugVertex), m_verts.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        // Orphan the buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
        // Upload the data
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_numElements = m_indices.size();
        m_indices.clear();
        m_verts.clear();
    }

    glm::vec2 rotatePoint(glm::vec2 position, float angle)
    {
        glm::vec2 newVec;

        newVec.x = (float) (position.x * cos(angle) - position.y * sin(angle));
        newVec.y = (float) (position.x * sin(angle) + position.y * cos(angle));

        return newVec;
    }

    void DebugRenderer::drawBox(const glm::vec4 &destRect, const Color &color, float angle)
    {
        int i = m_verts.size();
        m_verts.resize(m_verts.size() + 4);

        glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

        glm::vec2 tl(-halfDims.x, halfDims.y);
        glm::vec2 bl(-halfDims.x, -halfDims.y);
        glm::vec2 br(halfDims.x, -halfDims.y);
        glm::vec2 tr(halfDims.x, halfDims.y);

        glm::vec2 positionOffset(destRect.x, destRect.y);

        m_verts[i].position = rotatePoint(tl, angle) + halfDims + positionOffset;
        m_verts[i + 1].position = rotatePoint(bl, angle) + halfDims + positionOffset;
        m_verts[i + 2].position = rotatePoint(br, angle) + halfDims + positionOffset;
        m_verts[i + 3].position = rotatePoint(tr, angle) + halfDims + positionOffset;

        for (int j = i; j < i + 4; j++)
        {
            m_verts[j].color = color;
        }

        m_indices.reserve(m_indices.size() + 8);

        m_indices.push_back(i);
        m_indices.push_back(i + 1);

        m_indices.push_back(i + 1);
        m_indices.push_back(i + 2);

        m_indices.push_back(i + 2);
        m_indices.push_back(i + 3);

        m_indices.push_back(i + 3);
        m_indices.push_back(i);
    }

    void DebugRenderer::drawCircle(const glm::vec2 &center, const Color &color, float radius)
    {
        static const int NUM_VERTS = 30;

        // Set up vertices
        int start = m_verts.size();
        m_verts.resize(m_verts.size() + NUM_VERTS);

        for (int i = 0; i < NUM_VERTS; i++)
        {
            float angle = ((float) i / NUM_VERTS) * M_PI * 2.0f;
            m_verts[start + i].position.x = cos(angle) * radius + center.x;
            m_verts[start + i].position.y = sin(angle) * radius + center.y;
            m_verts[start + i].color = color;
        }

        // Set up indices for indexed drawing
        m_indices.reserve(m_indices.size() + NUM_VERTS * 2);
        for (int i = 0; i < NUM_VERTS - 1; i++)
        {
            m_indices.push_back(start + i);
            m_indices.push_back(start + i + 1);
        }
        m_indices.push_back(start + NUM_VERTS - 1);
        m_indices.push_back(start);
    }

    void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth)
    {
        m_shaderProgram.use();

        GLint pUniform = m_shaderProgram.getUniformLocation("P");
        glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

        glLineWidth(lineWidth);

        glBindVertexArray(m_vao);
        glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        m_shaderProgram.unuse();

    }

    void DebugRenderer::dispose()
    {
        if (m_vao) {
            glDeleteVertexArrays(1, &m_vao);
        }
        if (m_vbo) {
            glDeleteBuffers(1, &m_vbo);
        }
        if (m_ibo) {
            glDeleteBuffers(1, &m_ibo);
        }
        m_shaderProgram.dispose();
    }
}
