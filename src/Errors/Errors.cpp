#include "Errors.h"
#include <iostream>
#include "SDL2/SDL.h"
#include <cstdlib>
namespace Falcon {
    void fatalError(std::string error) {
        std::cout << error << std::endl;
        SDL_Quit();
        exit(-1);
    }
}