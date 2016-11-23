#include "GLSLProgram.h"
#include "../Errors/Errors.h"
#include <fstream>
#include <vector>
namespace Falcon {
    void GLSLProgram::compileShaders(const std::string &vertShaderPath, const std::string &fragShaderPath) {
        //Get a program object.
        m_programID = glCreateProgram();

        m_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
        if (m_vertShaderID == 0) {
            fatalError("Vertex shader failed to be created");
        }

        m_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if (m_fragShaderID == 0) {
            fatalError("Fragment shader failed to be created");
        }

        compileShader(vertShaderPath, m_vertShaderID);
        compileShader(fragShaderPath, m_fragShaderID);

    }

    void GLSLProgram::linkShaders() {

        //Vertex and fragment shaders are successfully compiled.
        //Now time to link them together into a program.
        //Attach our shaders to our program
        glAttachShader(m_programID, m_vertShaderID);
        glAttachShader(m_programID, m_fragShaderID);

        //Link our program
        glLinkProgram(m_programID);

        //Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_programID, GL_LINK_STATUS, (int *) &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

            //The maxLength includes the NULL character
            std::vector<GLchar> errorLog(maxLength);
            glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

            //We don't need the program anymore.
            glDeleteProgram(m_programID);
            //Don't leak shaders either.
            glDeleteShader(m_vertShaderID);
            glDeleteShader(m_fragShaderID);

            //Use the errorLog as you see fit.
            std::printf("%s\n", &errorLog[0]);
            fatalError("Shaders failed to link");
            //In this simple program, we'll just leave
            return;
        }

        //Always detach shaders after a successful link.
        glDetachShader(m_programID, m_vertShaderID);
        glDetachShader(m_programID, m_fragShaderID);
        glDeleteShader(m_vertShaderID);
        glDeleteShader(m_fragShaderID);
    }

    void GLSLProgram::compileShader(const std::string &shaderPath, GLuint id) {

        std::ifstream shaderFile(shaderPath);
        if (shaderFile.fail()) {
            fatalError("Failed to open" + shaderPath);
        }

        std::string fileContents = "";
        std::string line;

        while (std::getline(shaderFile, line)) {
            fileContents += line + "\n";
        }
        shaderFile.close();

        const char *contentsPtr = fileContents.c_str();
        glShaderSource(id, 1, &contentsPtr, nullptr);
        glCompileShader(id);

        GLint success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

            // Provide the infolog in whatever manor you deem best.
            // Exit with failure.
            glDeleteShader(id); // Don't leak the shader.

            std::printf("%s\n", &errorLog[0]);
            fatalError("Shader " + shaderPath + " failed to compile");
        }

    }

    void GLSLProgram::addAttribute(const std::string &attributeName) {
        glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
    }

    void GLSLProgram::use() {
        glUseProgram(m_programID);
        for (GLuint i = 0; i < m_numAttributes; i++) {
            glEnableVertexAttribArray(i);
        }
    }

    void GLSLProgram::unuse() {
        glUseProgram(0);
        for (GLuint i = 0; i < m_numAttributes; i++) {
            glDisableVertexAttribArray(i);
        }
    }

    GLint GLSLProgram::getUniformLocation(const std::string &uniformName) {
        GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
        if (location == GL_INVALID_INDEX) {
            fatalError("Uniform " + uniformName + " not found in shader!");
        }
        return location;
    }
}