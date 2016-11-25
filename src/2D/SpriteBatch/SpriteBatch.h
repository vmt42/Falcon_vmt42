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

    struct Glyph
    {
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

        static bool cmpFTB(Glyph a, Glyph b);
        static bool cmpBTF(Glyph a, Glyph b);
        static bool cmpTXT(Glyph a, Glyph b);
    private:
        GlyphSortType m_sortType;
        GLuint m_vbo; // Vertex buffer object
        GLuint m_vao; // Vertex array object
        std::vector<Glyph> m_glyphs;
        std::vector<RenderBatch> m_renderBatches;
    };
}




#endif
