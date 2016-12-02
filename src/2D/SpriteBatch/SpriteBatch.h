#ifndef FALCONLATINUM_SPRITEBATCH_H
#define FALCONLATINUM_SPRITEBATCH_H

#include <SDL2/SDL.h>
//#include "SDL2/SDL_opengl.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Vertex.h"
#include <GLES2/gl2.h>

enum class GlyphSortType
{
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

namespace Falcon
{

    class Glyph
    {
    public:
        Glyph() {};
        Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture, float Depth, const Color &color)
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
        GLuint texture;
        float depth;
        Vertex topLeft;
        Vertex bottomLeft;
        Vertex topRight;
        Vertex bottomRight;
    };

    class RenderBatch
    {
    public:
        RenderBatch(GLuint _offset, GLuint _num, GLuint _texture) : offset(_offset), numVertices(_num), texture(_texture) {};
        GLuint offset;
        GLuint numVertices;
        GLuint texture;
    };

    class SpriteBatch
    {
    public:
        SpriteBatch() : m_vao(0), m_vbo(0) {};
        ~SpriteBatch() {};

        void init();

        void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
        void end();

        void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const Color &color);

        void renderBatch();

    private:
        void createRenderBatches();
        void createVertexArray();
        void sortGlyphs();

        static bool cmpFTB(Glyph* a, Glyph* b);
        static bool cmpBTF(Glyph* a, Glyph* b);
        static bool cmpTXT(Glyph* a, Glyph* b);
    private:
        GlyphSortType m_sortType;
        GLuint m_vbo; // Vertex buffer object
        GLuint m_vao; // Vertex array object

        std::vector<Glyph*> m_glyphPtrs; // Vector for sorting
        std::vector<Glyph> m_glyphs;
        std::vector<RenderBatch> m_renderBatches;
    };
}




#endif
