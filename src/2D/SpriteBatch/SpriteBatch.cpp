#include <GL/glew.h>
#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>

namespace Falcon
{

    void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const Color &color)
    {
        m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
    }

    void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth,
                           const Color &color, float angle)
    {
        m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
    }

    void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth,
                           const Color &color, const glm::vec2 dir)
    {
        const glm::vec2 right(1.0f, 0.0f);
        float angle = acos(glm::dot(right, dir));

        if (dir.y < 0.0f) angle = -angle;


        m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
    }

    void SpriteBatch::init()
    {
        createVertexArray();
    }

    void SpriteBatch::begin(GlyphSortType sortType)
    {
        m_sortType = sortType;
        m_renderBatches.clear();
        m_glyphs.clear();
    }

    void SpriteBatch::end()
    {
        m_glyphPtrs.resize(m_glyphs.size());
        for (int i = 0; i < m_glyphs.size(); i++)
        {
            m_glyphPtrs[i] = &m_glyphs[i];
        }

        sortGlyphs();
        createRenderBatches();
    }

    void SpriteBatch::renderBatch()
    {
        glBindVertexArray(m_vao);
        for (int i = 0; i < m_renderBatches.size(); i++)
        {
            glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
        }
        glBindVertexArray(0);
    }

    void SpriteBatch::createRenderBatches()
    {
        std::vector<Vertex> vertices;
        vertices.resize(m_glyphPtrs.size() * 6);
        if (m_glyphPtrs.empty())
            return;


        int offset = 0;

        m_renderBatches.emplace_back(offset, 6, m_glyphPtrs[0]->texture);
        int cV = 0; // Current vertex
        vertices[cV++] = m_glyphPtrs[0]->topLeft;
        vertices[cV++] = m_glyphPtrs[0]->bottomLeft;
        vertices[cV++] = m_glyphPtrs[0]->bottomRight;
        vertices[cV++] = m_glyphPtrs[0]->bottomRight;
        vertices[cV++] = m_glyphPtrs[0]->topRight;
        vertices[cV++] = m_glyphPtrs[0]->topLeft;
        offset += 6;

        for (int cg = 1; cg < m_glyphPtrs.size(); cg++)
        {
            if (m_glyphPtrs[cg]->texture != m_glyphPtrs[cg - 1]->texture)
                m_renderBatches.emplace_back(offset, 6, m_glyphPtrs[cg]->texture);
            else
                m_renderBatches.back().numVertices += 6;

            vertices[cV++] = m_glyphPtrs[cg]->topLeft;
            vertices[cV++] = m_glyphPtrs[cg]->bottomLeft;
            vertices[cV++] = m_glyphPtrs[cg]->bottomRight;
            vertices[cV++] = m_glyphPtrs[cg]->bottomRight;
            vertices[cV++] = m_glyphPtrs[cg]->topRight;
            vertices[cV++] = m_glyphPtrs[cg]->topLeft;
            offset += 6;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        //Orphan the buffer
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        //Upload the data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SpriteBatch::createVertexArray()
    {
        if (m_vao == 0)
            glGenVertexArrays(1, &m_vao);

        glBindVertexArray(m_vao);

        if (m_vbo == 0)
            glGenBuffers(1, &m_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Position attribute pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
        // Color attribute pointer
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, color));
        // Uv attribute pointer
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));


        glBindVertexArray(0);
    }

    void SpriteBatch::sortGlyphs()
    {
        switch (m_sortType)
        {
            case GlyphSortType::BACK_TO_FRONT:
                std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), cmpBTF);
                break;
            case GlyphSortType::FRONT_TO_BACK:
                std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), cmpFTB);
                break;
            case GlyphSortType::TEXTURE:
                std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), cmpTXT);
                break;
        }

    }

    bool SpriteBatch::cmpFTB(Glyph* a, Glyph* b)
    {
        return (a->depth < b->depth);
    }

    bool SpriteBatch::cmpBTF(Glyph* a, Glyph* b)
    {
        return (a->depth > b->depth);
    }

    bool SpriteBatch::cmpTXT(Glyph* a, Glyph* b)
    {
        return (a->texture < b->texture);
    }


    Glyph::Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture, float Depth, const Color &color)
            : texture(Texture), depth(Depth)
    {
        topLeft.color = color;
        topLeft.setPosition(destRect.x, destRect.y + destRect.w);
        topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

        bottomLeft.color = color;
        bottomLeft.setPosition(destRect.x, destRect.y);
        bottomLeft.setUV(uvRect.x, uvRect.y);

        bottomRight.color = color;
        bottomRight.setPosition(destRect.x + destRect.z , destRect.y);
        bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

        topRight.color = color;
        topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
        topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    }

    Glyph::Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture, float Depth, const Color &color,
                 float angle)
            : texture(Texture), depth(Depth)
    {
        glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

        //Get points set at the origin
        glm::vec2 tl(-halfDims.x, halfDims.y);
        glm::vec2 bl(-halfDims.x, -halfDims.y);
        glm::vec2 br(halfDims.x, -halfDims.y);
        glm::vec2 tr(halfDims.x, halfDims.y);

        //Rotate the points
        tl = rotatePoint(tl, angle) + halfDims;
        bl = rotatePoint(bl, angle) + halfDims;
        br = rotatePoint(br, angle) + halfDims;
        tr = rotatePoint(tr, angle) + halfDims;

        topLeft.color = color;
        topLeft.setPosition(destRect.x + tl.x, destRect.y + destRect.w + tl.y);
        topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

        bottomLeft.color = color;
        bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
        bottomLeft.setUV(uvRect.x, uvRect.y);

        bottomRight.color = color;
        bottomRight.setPosition(destRect.x + destRect.z + br.x, destRect.y + br.y);
        bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

        topRight.color = color;
        topRight.setPosition(destRect.x + destRect.z + tr.x, destRect.y + destRect.w + tr.y);
        topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    }

    glm::vec2 Glyph::rotatePoint(glm::vec2 position, float angle)
    {
        glm::vec2 newVec;

        newVec.x = (float) (position.x * cos(angle) - position.y * sin(angle));
        newVec.y = (float) (position.x * sin(angle) + position.y * cos(angle));

        return newVec;
    }
}