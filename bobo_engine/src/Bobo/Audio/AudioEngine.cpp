#include "bobopch.h"

#include "AudioEngine.h"

// Code is also adapted from: https://github.com/rosshoyt/FMOD-Audio-Engine/tree/master

namespace Bobo {
    void CheckForFMODError(FMOD_RESULT result, const char* file, int line)
    {
        if (result != FMOD_OK) 
        {
            BOBO_ERROR("FMOD Error [Line {}]: Result - {}: {}", line, result, FMOD_ErrorString(result));
        }
    }

    AudioEngine::AudioEngine() : m_Sounds(), m_LoopsPlaying(), m_SoundBanks() // eventDescriptions(), eventInstances() 
    {
        // Initialize on Construction
        Init();
    }

    void AudioEngine::Init() 
    {
        ERRCHECK(FMOD::Studio::System::create(&p_StudioSystem));
        ERRCHECK(p_StudioSystem->getCoreSystem(&p_LowLevelSystem));
        ERRCHECK(p_LowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_STEREO, 0));
        ERRCHECK(p_LowLevelSystem->set3DSettings(1.0, DISTANCEFACTOR, 1.0f));
        ERRCHECK(p_StudioSystem->initialize(MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
    }
    
    void AudioEngine::Update() 
    {
        ERRCHECK(p_StudioSystem->update()); // also updates the low level system
    }

    void AudioEngine::LoadSound(SoundInfo soundInfo) 
    {
        if (!SoundLoaded(soundInfo)) 
        {
            FMOD::Sound* sound;
            ERRCHECK(p_LowLevelSystem->createSound(soundInfo.m_FilePath, soundInfo.m_Is3D ? FMOD_3D : FMOD_2D, 0, &sound));
            ERRCHECK(sound->setMode(soundInfo.m_IsLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
            ERRCHECK(sound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR));
            m_Sounds.insert({ soundInfo.m_UniqueID, sound });
            BOBO_INFO("Loaded Sound from File {}", soundInfo.m_FilePath);
        }
        else 
        {
            BOBO_WARN("Loading Sound from File {} Failed, Sound was Already Loaded");
        }
    }

    void AudioEngine::PlaySound(SoundInfo soundInfo) 
    {
        if (SoundLoaded(soundInfo)) 
        {
            FMOD::Channel* channel;

            // start play in 'paused' state
            ERRCHECK(p_LowLevelSystem->playSound(m_Sounds[soundInfo.m_UniqueID], 0, true, &channel));

            if (soundInfo.m_Is3D)
                Set3dChannelPosition(soundInfo, channel);

            if (soundInfo.m_IsLoop) // add to channel map of sounds currently playing, to stop later
                m_LoopsPlaying.insert({ soundInfo.m_UniqueID, channel });

            // start audio playback
            ERRCHECK(channel->setPaused(false));
        }
        else 
        {
            BOBO_WARN("Failed to Play Sound {}, Remember to Load the Sound first using .LoadSound", soundInfo.m_FilePath);
        }
    }

    
    void AudioEngine::StopSound(SoundInfo soundInfo) 
    {
        if (SoundIsPlaying(soundInfo)) 
        {
            m_LoopsPlaying[soundInfo.m_UniqueID]->stop();
            m_LoopsPlaying.erase(soundInfo.m_UniqueID);
        }
        else
        {
            BOBO_WARN("Failed to Stop Sound {}, Can't Stop a Sound that's not Playing", soundInfo.m_FilePath);
        }
    }

    bool AudioEngine::SoundIsPlaying(SoundInfo soundInfo) 
    {
        return soundInfo.m_IsLoop && m_LoopsPlaying.count(soundInfo.m_UniqueID);
    }

    void AudioEngine::Update3DSoundPosition(SoundInfo soundInfo) 
    {
        if (SoundIsPlaying(soundInfo))
        {
            Set3dChannelPosition(soundInfo, m_LoopsPlaying[soundInfo.m_UniqueID]);
        }
        else
        {
            BOBO_WARN("Failed to Update Sound Position for {}, Can't Update Sound Position of a Sound that's not Playing", soundInfo.m_FilePath);
        }
    }

    void AudioEngine::Set3DListenerPosition(float posX, float posY, float posZ, 
                                            float forwardX, float forwardY, float forwardZ, 
                                            float upX, float upY, float upZ) 
    {
        m_ListenerPos = { posX,     posY,       posZ };
        m_Forward =     { forwardX, forwardY,   forwardZ };
        m_Up =          { upX,      upY,        upZ };
        ERRCHECK(p_LowLevelSystem->set3DListenerAttributes(0, &m_ListenerPos, 0, &m_Forward, &m_Up));
    }

    bool AudioEngine::SoundLoaded(SoundInfo soundInfo) 
    {
        return m_Sounds.count(soundInfo.m_UniqueID) > 0;
    }

    void AudioEngine::Set3dChannelPosition(SoundInfo soundInfo, FMOD::Channel* channel) 
    {
        FMOD_VECTOR position = { 
            soundInfo.m_PosInfo.x * DISTANCEFACTOR, 
            soundInfo.m_PosInfo.y * DISTANCEFACTOR, 
            soundInfo.m_PosInfo.z * DISTANCEFACTOR };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
        ERRCHECK(channel->set3DAttributes(&position, &velocity));
    }
}