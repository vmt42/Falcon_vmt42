#include "Falcon.h"
#include "SDL2/SDL.h"
#include <GL/glew.h>

void Falcon::init()
{

    SDL_Init(SDL_INIT_EVERYTHING); // Start up init

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Enable double buffer
}
