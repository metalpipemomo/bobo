#pragma once

#include "../Component.h"
#include "../../Audio/Audio.h"
#include <unordered_map>

class AudioSource : public Component
{
public:
	bool m_Muted;
	float m_Pitch;
	float m_Vol;
	float m_Pan;
	
	AudioSource() { SetDefaultValues(); Set2D(); };
	AudioSource(Transform* transform) { SetDefaultValues(); Set3D(transform); };

	void SetDefaultValues()
	{
		m_Muted = false;
		m_Vol = 1;
		m_Pitch = 1;
		m_Pan = 0;
	}

	virtual void Play(const char* identifier)
	{
		// Make sure sound is loaded
		if (!Audio::HasLoadedSound(identifier)) return;

		auto soundInfo = Audio::GetSoundInfo(identifier);

		// Set sound properrties
		soundInfo->m_Is3D = m_Is3D;
		if (m_Is3D)
		{
			soundInfo->Set3DCoords(p_Transform->position);
		}
		else
		{
			soundInfo->m_Pan = m_Pan;
		}
		soundInfo->m_Muted = m_Muted;
		soundInfo->m_Pitch = m_Pitch;
		soundInfo->SetVolume(m_Vol);

		// Play sound
		Audio::PlaySound(identifier);
	}

	void Set2D()
	{
		p_Transform = nullptr;
		m_Is3D = false;
	}

	void Set3D(Transform* transform)
	{
		if (transform == nullptr) return;
		p_Transform = transform;
		m_Is3D = true;
	}
private:
	const Transform* p_Transform;
	bool m_Is3D;
};