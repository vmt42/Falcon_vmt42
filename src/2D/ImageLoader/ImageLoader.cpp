#include <GLES2/gl2.h>
#include "ImageLoader.h"
#include "../picoPNG.h"
#include "../../InputOutput/IOManager.h"
#include "../../Errors/Errors.h"
namespace Falcon
{
    GLTexture ImageLoader::loadPNG(std::string &filePath) {
        GLTexture texture = {};
        unsigned long width, height;

        std::vector<unsigned char> inputData;
        std::vector<unsigned char> outputData;

        if (!IOManager::readFileToBuffer(filePath, inputData)) {
            fatalError("Failed to load png file to buffer");
        }

        int error = decodePNG(outputData, width, height, &(inputData[0]), inputData.size());
        if (error != 0) {
            fatalError("DecodePNG failed with error: " + error);
        }

        glGenTextures(1, &(texture.id)); // Generate texture

        glBindTexture(GL_TEXTURE_2D, texture.id); // Bind texture

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     &(outputData[0])); // Upload texutre

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        texture.width = width;
        texture.height = height;

        return texture;
    }
}