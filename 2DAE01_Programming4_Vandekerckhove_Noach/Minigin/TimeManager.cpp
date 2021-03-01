#include "MiniginPCH.h"
#include "TimeManager.h"

using namespace dae;

void TimeManager::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;
	m_ElapsedTime += deltaTime;

	//FPS
	m_FPS = unsigned int(1.0f / deltaTime);
	//m_FPSTimer += deltaTime;
	//m_FPSCounter++;
	//if (m_FPSTimer >= 1.0f)
	//{
	//	m_FPS = m_FPSCounter;
	//	m_FPSCounter = 0;
	//	m_FPSTimer -= 1.0f;
	//}
}