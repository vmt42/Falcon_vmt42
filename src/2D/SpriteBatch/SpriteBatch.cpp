#include <GL/glew.h>
#include "SpriteBatch.h"
#include <algorithm>
namespace Falcon
{

    void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const Color &color)
    {
        Glyph newGlyph;
        newGlyph.texture = texture;
        newGlyph.depth = depth;

        newGlyph.topLeft.color = color;
        newGlyph.topLeft.setPosition(destRect.x, destRect.y + destRect.w);
        newGlyph.topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

        newGlyph.bottomLeft.color = color;
        newGlyph.bottomLeft.setPosition(destRect.x, destRect.y);
        newGlyph.bottomLeft.setUV(uvRect.x, uvRect.y);

        newGlyph.bottomRight.color = color;
        newGlyph.bottomRight.setPosition(destRect.x + destRect.z , destRect.y);
        newGlyph.bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

        newGlyph.topRight.color = color;
        newGlyph.topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
        newGlyph.topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

        m_glyphs.push_back(newGlyph);

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
        sortGlyphs();
        createRenderBatches();
    }

    void SpriteBatch::renderBatch()
    {
        glBindVertexArray(m_vao);
        for (int i = 0; i < m_renderBatches.size(); i++)
        {
            glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
            glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
        }
        glBindVertexArray(0);
    }

    void SpriteBatch::createRenderBatches()
    {
        std::vector<Vertex> vertices;
        vertices.resize(m_glyphs.size() * 6);
        if (m_glyphs.empty())
            return;


        int offset = 0;

        m_renderBatches.emplace_back(offset, 6, m_glyphs[0].texture);
        int cV = 0; // Current vertex
        vertices[cV++] = m_glyphs[0].topLeft;
        vertices[cV++] = m_glyphs[0].bottomLeft;
        vertices[cV++] = m_glyphs[0].bottomRight;
        vertices[cV++] = m_glyphs[0].bottomRight;
        vertices[cV++] = m_glyphs[0].topRight;
        vertices[cV++] = m_glyphs[0].topLeft;
        offset += 6;

        for (int cg = 1; cg < m_glyphs.size(); cg++)
        {
            if (m_glyphs[cg].texture != m_glyphs[cg - 1].texture)
                m_renderBatches.emplace_back(offset, 6, m_glyphs[cg].texture);
            else
                m_renderBatches.back().numVertices += 6;

            vertices[cV++] = m_glyphs[cg].topLeft;
            vertices[cV++] = m_glyphs[cg].bottomLeft;
            vertices[cV++] = m_glyphs[cg].bottomRight;
            vertices[cV++] = m_glyphs[cg].bottomRight;
            vertices[cV++] = m_glyphs[cg].topRight;
            vertices[cV++] = m_glyphs[cg].topLeft;
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
                std::stable_sort(m_glyphs.begin(), m_glyphs.end(), cmpBTF);
                break;
            case GlyphSortType::FRONT_TO_BACK:
                std::stable_sort(m_glyphs.begin(), m_glyphs.end(), cmpFTB);
                break;
            case GlyphSortType::TEXTURE:
                std::stable_sort(m_glyphs.begin(), m_glyphs.end(), cmpTXT);
                break;
        }

    }

    bool SpriteBatch::cmpFTB(Glyph a, Glyph b)
    {
        return (a.depth < b.depth);
    }

    bool SpriteBatch::cmpBTF(Glyph a, Glyph b)
    {
        return (a.depth > b.depth);
    }

    bool SpriteBatch::cmpTXT(Glyph a, Glyph b)
    {
        return (a.texture < b.texture);
    }


}