#include <GL/glew.h>
#include "SpriteBatch.h"
#include <algorithm>
namespace Falcon
{

    void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const Color &color)
    {
        m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
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


}