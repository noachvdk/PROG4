#pragma once
#include "Singleton.h"

namespace dae
{
	class TimeManager final : public Singleton<TimeManager> 
	{
	public:
		void Update(float deltaTime);
		unsigned int GetFPS() const { return m_FPS; }
		float GetDeltaTime() const { return m_DeltaTime; }

	private:
		friend class Singleton<TimeManager>;
		TimeManager() = default;
		//Time
		float m_DeltaTime{};
		float m_ElapsedTime{};
		//FPS
		unsigned int m_FPS{};
		//unsigned int m_FPSCounter{};
		//float m_FPSTimer{};
	};

}


