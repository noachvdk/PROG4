#pragma once
#include <iostream>
#include <mutex>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "audio.h"

namespace dae
{
	struct Audio
	{
		std::string path;
		int volume;
		bool isMusic;
	};
	
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void PlaySound(const std::string& path, const int volume) = 0;
		virtual void PlayMusic(const std::string& path, const int volume) = 0;
		virtual void Pause() = 0;
		virtual void UnPause() = 0;
		virtual void TogglePause() = 0;
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Mute() = 0;
		virtual void UnMute() = 0;
		virtual void ToggleMute() = 0;
	};

	class NullAudioSystem : public SoundSystem
	{
	public:
		void PlayMusic(const std::string& , const int ) override
		{
			std::cout << "no SoundSystem registered\n";
		}
		void PlaySound(const std::string& , const int ) override
		{
			std::cout << "no SoundSystem registered\n";
		}
		void Pause() override { std::cout << "no sound system\n"; }
		void UnPause() override { std::cout << "no sound system\n"; }
		void TogglePause() override { std::cout << "no sound system\n"; }
		void Init() override { std::cout << "no sound system\n"; }
		void Update() override { }
		void Mute() override { std::cout << "Mute\n"; }
		void UnMute() override { std::cout << "UnMute\n"; }
		void ToggleMute() override { std::cout << "Mute\n"; }
	};
	
	class LoggingSoundSystem : public SoundSystem
	{
	public:
		LoggingSoundSystem(SoundSystem* ss) : m_pSoundSystem(ss){}
		~LoggingSoundSystem() { delete m_pSoundSystem; m_pSoundSystem = nullptr; };
		void PlayMusic(const std::string& path, const int volume) override
		{
			std::cout << "playing music " << path << "at volume " << volume << "\n";
			m_pSoundSystem->PlayMusic(path, volume);
		}
		void PlaySound(const std::string& path, const int volume) override
		{
			std::cout << "playing sound " << path << "at volume " << volume << "\n";
			m_pSoundSystem->PlaySound(path, volume);
		}
		void Pause() override { std::cout << "pausing\n"; m_pSoundSystem->Pause(); }
		void UnPause() override { std::cout << "unpausing\n"; m_pSoundSystem->UnPause(); }
		void TogglePause() override { std::cout << "toggle pausing\n"; m_pSoundSystem->TogglePause();
		}
		void Init() override { std::cout << "init soundsystem\n"; m_pSoundSystem->Init(); }
		void Update() override { m_pSoundSystem->Update(); }

		void Mute() override { std::cout << "Mute\n"; m_pSoundSystem->Mute(); }
		void UnMute() override { std::cout << "UnMute\n"; m_pSoundSystem->UnMute(); }
		void ToggleMute() override { std::cout << "toggle Mute\n"; m_pSoundSystem->ToggleMute(); }
	private:
		SoundSystem* m_pSoundSystem;
	};
	
	//class OpenGLAudioSystem : public SoundSystem
	//{
	//public:
	//	void Init() override
	//	{
	//		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	//		{
	//			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	//		}
	//		m_pTestSound = Mix_LoadWAV("../Data/sounds/door1.wav");
	//			if (m_pTestSound == nullptr)
	//				printf("Failed to load  sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	//	}
	//	void PlaySound(const std::string path, const int volume) override
	//	{
	//		Mix_PlayChannel(-1, m_pTestSound, 0);
	//		UNREFERENCED_PARAMETER(path);
	//		UNREFERENCED_PARAMETER(volume);
	//	}
	//	void PlayMusic(const std::string path, const int volume) override
	//	{
	//		Mix_PlayChannel(-1, m_pTestSound, -1);
	//		UNREFERENCED_PARAMETER(path);
	//		UNREFERENCED_PARAMETER(volume);
	//	}
	//	
	//private:
	//	Mix_Chunk* m_pTestSound;
	//};
}
