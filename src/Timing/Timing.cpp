#include "Timing.h"
#include "SDL2/SDL.h"

namespace Falcon
{

    void FPSLimiter::init(float targetFPS)
    {
        setTargetFPS(targetFPS);
    }

    void FPSLimiter::begin()
    {
        m_startTicks = SDL_GetTicks();
    }

    float FPSLimiter::end()
    {
        calculateFPS();

        float frameTicks = SDL_GetTicks() - m_startTicks;
        //Limit FPS to the maxFPS
        if (1000.0f / m_targetFPS > frameTicks)
        {
            SDL_Delay((Uint32) (1000.0f / m_targetFPS - frameTicks));
        }
        return m_FPS;
    }

    void FPSLimiter::setTargetFPS(float targetFPS)
    {
        m_targetFPS = targetFPS;
    }

    void FPSLimiter::calculateFPS()
    {
        static const int NUM_SAMPLES = 100; //How many frames we average
        static float frameTimes[NUM_SAMPLES];
        static int currentFrame = 0;
        static float prevTicks = SDL_GetTicks();


        float curTicks;

        curTicks = SDL_GetTicks();
        m_frameTime = curTicks - prevTicks;
        prevTicks = curTicks;
        frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

        int count;
        currentFrame++;

        if (currentFrame < NUM_SAMPLES)
            count = currentFrame;
        else
            count = NUM_SAMPLES;

        float frameTimeAverage = 0;
        for (int i = 0; i < count; i++)
        {
            frameTimeAverage += frameTimes[i];
        }
        frameTimeAverage /= count;

        m_FPS = frameTimeAverage > 0 ? (1000.0f / frameTimeAverage): 60.0f;
    }
}