#ifndef FALCONLATINUM_GLSLPROGRAM_H
#define FALCONLATINUM_GLSLPROGRAM_H

#include <string>
#include <GLES2/gl2.h>
#include "SDL2/SDL_opengl.h"
namespace Falcon {
    class GLSLProgram {
    public:
        GLSLProgram() : m_fragShaderID(0), m_vertShaderID(0), m_programID(0), m_numAttributes(0) {};

        ~GLSLProgram() {};

        void compileShaders(const std::string &vertShaderPath, const std::string &fragShaderPath);

        void linkShaders();

        void addAttribute(const std::string &attributeName);

        void use();

        void unuse();

        GLint getUniformLocation(const std::string &uniformName);

    private:
        void compileShader(const std::string &shaderPath, GLuint id);

    private:
        GLuint m_numAttributes;

        GLuint m_programID;
        GLuint m_vertShaderID;
        GLuint m_fragShaderID;
    };
}

#endif //FALCONLATINUM_GLSLPROGRAM_H
