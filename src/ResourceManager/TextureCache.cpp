#include <iostream>
#include "TextureCache.h"
#include "../2D/ImageLoader/ImageLoader.h"
namespace Falcon {
    GLTexture TextureCache::getTexture(std::string filePath) {
        auto texture = textureMap.find(filePath);
        if (texture == textureMap.end()) {
            // Load the texture
            GLTexture newTexture = ImageLoader::loadPNG(filePath);
            // Insert texture to the map
            textureMap.insert({filePath, newTexture});
            std::cout << "Loaded texture! \n";
            return newTexture;
        }
        std::cout << "Used cache texture! \n";
        return texture->second;
    }
}