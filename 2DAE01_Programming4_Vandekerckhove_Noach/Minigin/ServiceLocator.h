#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *m_SoundSystem; }
		static void RegisterSoundSystem(SoundSystem* ss)
		{
			m_SoundSystem = ((ss == nullptr) ? &m_DefaultSoundSystem : ss);
			m_SoundSystemRegistered = true;
			m_SoundSystem->Init();
		}
		static void Destroy()
		{
			if(m_SoundSystemRegistered) // whithout this check error at the delete pointer when not registered
				safeDelete(m_SoundSystem);
		}
	private:
		static SoundSystem* m_SoundSystem;
		static NullAudioSystem m_DefaultSoundSystem;
		static bool m_SoundSystemRegistered;
	};
}
