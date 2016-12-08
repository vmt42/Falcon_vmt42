#include "AudioEngine.h"
#include "../Errors/Errors.h"

namespace Falcon
{

    void AudioEngine::init()
    {
        if (Mix_Init(MIX_INIT_MP3 || MIX_INIT_OGG) == -1)
        {
            fatalError("Mix_Init error: " + std::string(Mix_GetError()));
        }

        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
        }
        m_isInit = true;
    }

    void AudioEngine::destroy()
    {
        if (m_isInit)
        {
            m_isInit = false;
            Mix_Quit();
        }
    }

    SoundEffect AudioEngine::loadSoundEffect(const std::string &filePath)
    {
        SoundEffect soundEffect;
        auto it = m_soundMap.find(filePath);
        if (it == m_soundMap.end())
        {
            Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
            if (chunk == nullptr)
            {
                fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
            }
            soundEffect.m_chunk = chunk;
            m_soundMap[filePath] = chunk;
        }
        else
        {
            soundEffect.m_chunk = it->second;
        }
        return  soundEffect;
    }

    Music AudioEngine::loadMusic(const std::string &filePath)
    {
        Music music;
        auto it = m_musicMap.find(filePath);
        if (it == m_musicMap.end())
        {
            Mix_Music* mix_music = Mix_LoadMUS(filePath.c_str());
            if (mix_music == nullptr)
            {
                fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
            }
            music.m_music = mix_music;
            m_musicMap[filePath] = mix_music;
        }
        else
        {
            music.m_music = it->second;
        }
        return  music;
    }

    void SoundEffect::play(int loops)
    {
        if (Mix_PlayChannel(-1, m_chunk, loops) == -1)
        {
            if (Mix_PlayChannel(0, m_chunk, loops) == -1)
            {
                fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
            }
        }
    }

    void Music::play(int loops)
    {
        Mix_PlayMusic(m_music, loops);
    }

    void Music::pause()
    {
        Mix_PauseMusic();
    }

    void Music::stop()
    {
        Mix_HaltMusic();
    }

    void Music::resume()
    {
        Mix_ResumeMusic();
    }
}
