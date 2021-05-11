#pragma once
#include "SoundSystem.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace dae
{
	class SDLAudioSystem : public SoundSystem
	{
	public:
		SDLAudioSystem()
			: m_IsMuted(false)
		{
			m_Thread = std::thread([this]() {this->Update(); });
		}
		~SDLAudioSystem()
		{
			m_Head = m_Tail;
			m_Cv.notify_one();
			if (m_Thread.joinable())
				m_Thread.join();
		}

		void PlaySound(const std::string path, const int volume) override
		{
			std::lock_guard<std::mutex>guard(m_Mutex);

			for (int i = m_Head; i != m_Tail; i = (i + 1) % MAX_PENDING)
			{
				if (m_Pending[i].path == path)
				{
					return;
				}
			}

			assert((m_Tail + 1) % MAX_PENDING != m_Head);

			m_Pending[m_Tail].path = path;
			m_Pending[m_Tail].volume = volume;
			m_Pending[m_Tail].isMusic = false;
			m_Tail = (m_Tail + 1) % MAX_PENDING;
			m_Cv.notify_one();
		}
		void PlayMusic(const std::string path, const int volume) override
		{
			std::lock_guard<std::mutex>guard(m_Mutex);
			assert((m_Tail + 1) % MAX_PENDING != m_Head);

			m_Pending[m_Tail].path = path;
			m_Pending[m_Tail].volume = volume;
			m_Pending[m_Tail].isMusic = true;

			m_Tail = (m_Tail + 1) % MAX_PENDING;
			m_Cv.notify_one();
		}

		void Pause() override { pauseAudio(); }
		void UnPause() override { unpauseAudio(); }

		void Init() override { m_Head = 0; m_Tail = 0; m_IsMuted = false; }

		void Mute() override { m_IsMuted = true; }
		void UnMute() override { m_IsMuted = false; }
		void ToggleMute() override { m_IsMuted = !m_IsMuted; }

		void Update() override
		{
			do
			{
				std::unique_lock<std::mutex> guard(m_Mutex); //unique lock because of the cv
				if (m_Head != m_Tail)
				{
					if (!m_IsMuted)
					{
						if (!m_Pending[m_Head].isMusic)
							playSound(m_Pending[m_Head].path.c_str(), m_Pending[m_Head].volume);
						else
							playMusic(m_Pending[m_Head].path.c_str(), m_Pending[m_Head].volume);
					}
					m_Head = (m_Head + 1) % MAX_PENDING;
				}

				if (m_Head == m_Tail)
					m_Cv.wait(guard);

			} while (m_Head != m_Tail);
		}
	private:
		static const int MAX_PENDING = 16;
		static Audio m_Pending[MAX_PENDING];
		static int m_Head;
		static int m_Tail;
		std::mutex m_Mutex;
		std::thread m_Thread;
		std::condition_variable m_Cv;
		bool m_IsMuted;
	};
}


