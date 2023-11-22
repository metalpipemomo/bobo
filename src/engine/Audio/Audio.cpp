#include "../../bpch.h"

#include "Audio.h"

void CheckForFMODError(FMOD_RESULT result, const char* file, int line)
{
    if (result != FMOD_OK)
    {
        BOBO_ERROR("FMOD Error [Line {}]: Result - {}: {}", line, result, FMOD_ErrorString(result));
    }
}

Audio::Audio() : m_Sounds(), m_LoopsPlaying(), m_SoundBanks() // eventDescriptions(), eventInstances() 
{
    // Initialize on Construction
    ERRCHECK(FMOD::Studio::System::create(&p_StudioSystem));
    ERRCHECK(p_StudioSystem->getCoreSystem(&p_LowLevelSystem));
    ERRCHECK(p_LowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_STEREO, 0));
    ERRCHECK(p_LowLevelSystem->set3DSettings(1.0, DISTANCEFACTOR, 1.0f));
    ERRCHECK(p_StudioSystem->initialize(MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
}

void Audio::Init()
{
    auto ae = GetInstance();
    ae->LoadAllSounds("../assets/Sounds");
}

void Audio::Update()
{
    auto ae = GetInstance();
    ERRCHECK(ae->p_StudioSystem->update()); // also updates the low level system
}

void Audio::LoadSound(SoundInfo soundInfo, const std::string& identifier)
{
    auto ae = GetInstance();
    if (!ae->SoundLoaded(soundInfo))
    {
        std::string copyIdentifier = identifier;
        std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::toupper);
        ae->m_SoundInfoStore.insert({ copyIdentifier, soundInfo });
        FMOD::Sound* sound;
        ERRCHECK(ae->p_LowLevelSystem->createSound(soundInfo.m_FilePath, soundInfo.m_Is3D ? FMOD_3D : FMOD_2D, 0, &sound));
        ERRCHECK(sound->setMode(soundInfo.m_IsLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
        ERRCHECK(sound->set3DMinMaxDistance(0.5f * ae->DISTANCEFACTOR, 5000.0f * ae->DISTANCEFACTOR));
        ae->m_Sounds.insert({ soundInfo.m_UniqueID, sound });
        BOBO_INFO("Loaded Sound from File {}", soundInfo.m_FilePath);
    } else
    {
        BOBO_WARN("Loading Sound from File {} Failed, Sound was Already Loaded");
    }
}

void Audio::PlaySoundI(SoundInfo soundInfo)
{
    auto ae = GetInstance();
    if (ae->SoundLoaded(soundInfo))
    {
        FMOD::Channel* channel;

        // start play in 'paused' state
        ERRCHECK(ae->p_LowLevelSystem->playSound(ae->m_Sounds[soundInfo.m_UniqueID], 0, true, &channel));

        if (soundInfo.m_Is3D)
            ae->Set3dChannelPosition(soundInfo, channel);

        if (soundInfo.m_IsLoop) // add to channel map of sounds currently playing, to stop later
            ae->m_LoopsPlaying.insert({ soundInfo.m_UniqueID, channel });

        // start audio playback
        ERRCHECK(channel->setPaused(false));
    } else
    {
        BOBO_WARN("Failed to Play Sound {}, Remember to Load the Sound first using .LoadSound", soundInfo.m_FilePath);
    }
}

void Audio::PlaySound(const std::string& identifier)
{
    auto ae = GetInstance();

    std::string copyIdentifier = identifier;
    std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::toupper);

    auto soundInfoIt = ae->m_SoundInfoStore.find(copyIdentifier);
    if (soundInfoIt != ae->m_SoundInfoStore.end())
    {
        SoundInfo* soundInfo = &soundInfoIt->second;

        if (ae->SoundLoaded(*soundInfo))
        {
            FMOD::Channel* channel;

            // start play in 'paused' state
            ERRCHECK(ae->p_LowLevelSystem->playSound(ae->m_Sounds[soundInfo->m_UniqueID], 0, true, &channel));

            if (soundInfo->m_Is3D)
                ae->Set3dChannelPosition(*soundInfo, channel);

            if (soundInfo->m_IsLoop) // add to channel map of sounds currently playing, to stop later
                ae->m_LoopsPlaying.insert({ soundInfo->m_UniqueID, channel });

            // start audio playback
            ERRCHECK(channel->setPaused(false));
        } else
        {
            BOBO_WARN("Failed to Play Sound {}, Remember to Load the Sound first using .LoadSound", soundInfo->m_FilePath);
        }
    }
}


void Audio::StopSound(SoundInfo soundInfo)
{
    auto ae = GetInstance();
    if (SoundIsPlaying(soundInfo))
    {
        ae->m_LoopsPlaying[soundInfo.m_UniqueID]->stop();
        ae->m_LoopsPlaying.erase(soundInfo.m_UniqueID);
    } else
    {
        BOBO_WARN("Failed to Stop Sound {}, Can't Stop a Sound that's not Playing", soundInfo.m_FilePath);
    }
}

bool Audio::SoundIsPlaying(SoundInfo soundInfo)
{
    auto ae = GetInstance();
    return soundInfo.m_IsLoop && ae->m_LoopsPlaying.count(soundInfo.m_UniqueID);
}

void Audio::Update3DSoundPosition(SoundInfo soundInfo)
{
    auto ae = GetInstance();
    if (SoundIsPlaying(soundInfo))
    {
        ae->Set3dChannelPosition(soundInfo, ae->m_LoopsPlaying[soundInfo.m_UniqueID]);
    } else
    {
        BOBO_WARN("Failed to Update Sound Position for {}, Can't Update Sound Position of a Sound that's not Playing", soundInfo.m_FilePath);
    }
}

void Audio::Set3DListenerPosition(float posX, float posY, float posZ,
    float forwardX, float forwardY, float forwardZ,
    float upX, float upY, float upZ)
{
    auto ae = GetInstance();
    ae->m_ListenerPos = { posX,     posY,       posZ };
    ae->m_Forward = { forwardX, forwardY,   forwardZ };
    ae->m_Up = { upX,      upY,        upZ };
    ERRCHECK(ae->p_LowLevelSystem->set3DListenerAttributes(0, &ae->m_ListenerPos, 0, &ae->m_Forward, &ae->m_Up));
}

int Audio::LoadAllSounds(const std::string& directory)
{
    int count = 0;
    for (const auto& dirEntry : std::filesystem::directory_iterator{ directory })
    {
        if (dirEntry.path().extension() != ".mp3") continue;
        auto path = dirEntry.path().generic_string();
        SoundInfo sf = SoundInfo(path.c_str(), false, false);
        auto identifier = dirEntry.path().stem().generic_string();
        LoadSound(sf, identifier);
        count++;
    }

    BOBO_INFO("Audio Engine loaded {} mp3 files from Directory {}", count, directory);

    return count;
}

bool Audio::SoundLoaded(SoundInfo soundInfo)
{
    return m_Sounds.count(soundInfo.m_UniqueID) > 0;
}

void Audio::Set3dChannelPosition(SoundInfo soundInfo, FMOD::Channel* channel)
{
    FMOD_VECTOR position = {
        soundInfo.m_PosInfo.x * DISTANCEFACTOR,
        soundInfo.m_PosInfo.y * DISTANCEFACTOR,
        soundInfo.m_PosInfo.z * DISTANCEFACTOR };
    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
    ERRCHECK(channel->set3DAttributes(&position, &velocity));
}