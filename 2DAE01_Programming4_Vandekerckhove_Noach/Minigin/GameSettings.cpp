#include "MiniginPCH.h"
#include "GameSettings.h"
#include "InputManager.h"
#include "Logger.h"
using namespace dae;

GameSettings::GameSettings()
	:m_CurrentGameMode(GameMode::NotChosen)
{
}


void GameSettings::SetGameMode(GameMode newMode)
{
	m_CurrentGameMode = newMode;
	if(m_CurrentGameMode == GameMode::SinglePlayer)
		Logger::GetInstance().Log(LogType::Info, "Gamemode is Singleplayer");
	else if(m_CurrentGameMode == GameMode::Coop)
		Logger::GetInstance().Log(LogType::Info, "Gamemode is Co-op");
	else if (m_CurrentGameMode == GameMode::Versus)
		Logger::GetInstance().Log(LogType::Info, "Gamemode is Versus");
}

