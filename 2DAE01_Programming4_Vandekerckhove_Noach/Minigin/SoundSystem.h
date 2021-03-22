#pragma once
#include <iostream>
#include <mutex>

//// SDL libs
//#pragma comment(lib, "sdl2.lib")
//#pragma comment(lib, "SDL2main.lib")
//
//// OpenGL libs
//#pragma comment (lib,"opengl32.lib")
//#pragma comment (lib,"Glu32.lib")
//
//// SDL extension libs 
//#pragma comment(lib, "SDL2_image.lib")  
//#pragma comment(lib, "SDL2_ttf.lib") 
//#pragma comment(lib, "SDL2_mixer.lib")  

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
		virtual void PlaySound(const std::string path, const int volume) = 0;
		virtual void PlayMusic(const std::string path, const int volume) = 0;
		virtual void Pause() = 0;
		virtual void UnPause() = 0;
		virtual void Init() = 0;
		virtual void Update() = 0;
	};

	class NullAudioSystem : public SoundSystem
	{
	public:
		void PlayMusic(const std::string path, const int volume) override
		{
			std::cout << "no SoundSystem registered\n";
			UNREFERENCED_PARAMETER(path);
			UNREFERENCED_PARAMETER(volume);
		}
		void PlaySound(const std::string path, const int volume) override
		{
			std::cout << "no SoundSystem registered\n";
			UNREFERENCED_PARAMETER(path);
			UNREFERENCED_PARAMETER(volume);
		}
		void Pause() override { std::cout << "no sound system\n"; }
		void UnPause() override { std::cout << "no sound system\n"; }
		void Init() override { std::cout << "no sound system\n"; }
		void Update() override { }
	};

	class SDLAudioSystem : public SoundSystem
	{
	public:
		void PlaySound(const std::string path, const int volume) override
		{
			std::lock_guard<std::mutex>guard(m_Mutex);

			for (int i = m_Head; i != m_Tail; i = (i + 1) % MAX_PENDING)
			{
				if(m_Pending[i].path == path)
				{
					return;
				}
			}
			
			assert((m_Tail + 1) % MAX_PENDING != m_Head);
			
			m_Pending[m_Tail].path = path;
			m_Pending[m_Tail].volume = volume;
			m_Pending[m_Tail].isMusic = false;
			m_Tail = (m_Tail + 1) % MAX_PENDING;
		}
		void PlayMusic(const std::string path, const int volume) override
		{
			std::lock_guard<std::mutex>guard(m_Mutex);
			assert((m_Tail + 1) % MAX_PENDING != m_Head);

			m_Pending[m_Tail].path = path;
			m_Pending[m_Tail].volume = volume;
			m_Pending[m_Tail].isMusic = true;
			
			m_Tail = (m_Tail + 1) % MAX_PENDING;
		}
		void Pause() override{ pauseAudio();}
		void UnPause() override { unpauseAudio(); }
		void Init() override {m_Head = 0; m_Tail = 0; }

		void Update() override
		{
			if (m_Head == m_Tail) //Return if no events in queue
				return;

			std::lock_guard<std::mutex> guard(m_Mutex);
			if (!m_Pending[m_Head].isMusic)
				playSound(m_Pending[m_Head].path.c_str(), m_Pending[m_Head].volume);
			else
				playMusic(m_Pending[m_Head].path.c_str(), m_Pending[m_Head].volume);

			m_Head = (m_Head + 1) % MAX_PENDING;
		}
	private:
		static const int MAX_PENDING = 16;
		static Audio m_Pending[MAX_PENDING];
		static int m_Head;
		static int m_Tail;
		std::mutex m_Mutex;
	};
	
	class LoggingSoundSystem : public SoundSystem
	{
	public:
		LoggingSoundSystem(SoundSystem* ss) : m_pSoundSystem(ss){}
		~LoggingSoundSystem() { delete m_pSoundSystem; m_pSoundSystem = nullptr; };
		void PlayMusic(const std::string path, const int volume) override
		{
			std::cout << "playing music " << path << "at volume " << volume << "\n";
			m_pSoundSystem->PlayMusic(path, volume);
		}
		void PlaySound(const std::string path, const int volume) override
		{
			std::cout << "playing sound " << path << "at volume " << volume << "\n";
			m_pSoundSystem->PlaySound(path, volume);
		}
		void Pause() override { std::cout << "pausing\n"; m_pSoundSystem->Pause(); }
		void UnPause() override { std::cout << "unpausing\n"; m_pSoundSystem->UnPause(); }
		void Init() override { std::cout << "init\n"; m_pSoundSystem->Init(); }
		void Update() override { m_pSoundSystem->Update(); }
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
