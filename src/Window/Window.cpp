#include "Window.h"
#include "../Errors/Errors.h"
namespace Falcon
{
    int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
    {

        Uint32 flags = SDL_WINDOW_OPENGL;
        m_screenHeight = screenHeight;
        m_screenWidth = screenWidth;

        if (currentFlags & INVISIBLE) {
            flags |= SDL_WINDOW_HIDDEN;
        }
        if (currentFlags & FULLSCREEN) {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        if (currentFlags & BORDERLESS) {
            flags |= SDL_WINDOW_BORDERLESS;
        }


        m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth,
                                       screenHeight, flags);
        if (m_sdlWindow == nullptr)
            fatalError("SDL window can't be created");

        // SDL OpenGL context window
        SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
        if (glContext == nullptr)
            fatalError("SDL_GL context could not be created");


        std::printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));
        std::printf("\n");
        glClearColor(44.0f / 255.0f, 62.0f / 255.0f, 80.0f / 255.0f, 1.0); // Set background color

        //Set VSYNC to 0
        SDL_GL_SetSwapInterval(0);

        //Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return 0;
    }

    void Window::swapBuffer() {
        SDL_GL_SwapWindow(m_sdlWindow);
    }
}