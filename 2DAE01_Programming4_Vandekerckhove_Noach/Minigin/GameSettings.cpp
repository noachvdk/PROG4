#include "MiniginPCH.h"
#include "GameSettings.h"
#include "InputManager.h"
#include "Logger.h"
#include "SceneManager.h"
using namespace dae;

GameSettings::GameSettings()
	:m_CurrentGameMode(GameMode::NotChosen)
{
}


void GameSettings::SetGameMode(GameMode newMode)
{
	m_CurrentGameMode = newMode;
	auto& input = InputManager::GetInstance();
	input.ResetInputActions();
	
	//inputs
	if(m_CurrentGameMode == GameMode::SinglePlayer)
	{
		Logger::GetInstance().Log(LogType::Info, "Gamemode is Singleplayer");
		
		//Player 1 inputs
		input.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonA, new MoveUpLeft() });
		input.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, new MoveUpRight() });
		input.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonY, new MoveDownLeft() });
		input.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonX, new MoveDownRight() });
	}
	else if(m_CurrentGameMode == GameMode::Coop)
	{
		Logger::GetInstance().Log(LogType::Info, "Gamemode is Co-op");
	
		//Player 1 inputs
		input.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonA, new MoveUpLeft() });
		input.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, new MoveUpRight() });
		input.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonY, new MoveDownLeft() });
		input.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonX, new MoveDownRight() });
		//Player 2 inputs
		input.AddInputAction(InputAction{ 1, SDLK_KP_7, TriggerType::OnPress, ControllerButton::ButtonA, new MoveUpLeft() });
		input.AddInputAction(InputAction{ 1, SDLK_KP_9, TriggerType::OnPress, ControllerButton::ButtonB, new MoveUpRight() });
		input.AddInputAction(InputAction{ 1, SDLK_KP_1, TriggerType::OnPress, ControllerButton::ButtonY, new MoveDownLeft() });
		input.AddInputAction(InputAction{ 1, SDLK_KP_3, TriggerType::OnPress, ControllerButton::ButtonX, new MoveDownRight() });
	}
	else if (m_CurrentGameMode == GameMode::Versus)
	{
		Logger::GetInstance().Log(LogType::Info, "Gamemode is Versus");
		//Player 1 inputs
		input.AddInputAction(InputAction{ 0, SDLK_z, TriggerType::OnPress, ControllerButton::ButtonA, new DieCommand() });
		input.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonB, new DefeatedCoilyCommand() });
		input.AddInputAction(InputAction{ 0, SDLK_s, TriggerType::OnPress, ControllerButton::ButtonY, new ColorChangeCommand() });
		input.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonX, new CatchSlickOrSamCommand() });
		//Player 2 inputs
		input.AddInputAction(InputAction{ 1, SDLK_UP, TriggerType::OnPress, ControllerButton::ButtonA, new DieCommand() });
		input.AddInputAction(InputAction{ 1, SDLK_LEFT, TriggerType::OnPress, ControllerButton::ButtonB, new DefeatedCoilyCommand() });
		input.AddInputAction(InputAction{ 1, SDLK_DOWN, TriggerType::OnPress, ControllerButton::ButtonY, new ColorChangeCommand() });
		input.AddInputAction(InputAction{ 1, SDLK_RIGHT, TriggerType::OnPress, ControllerButton::ButtonX, new CatchSlickOrSamCommand() });
	}
}

void GameSettings::SetGameOver()
{
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.SetCurrentSceneName("MainMenu");

	Logger::GetInstance().Log(LogType::Info, "Game over");
	
	auto& lvl = LevelManager::GetInstance();
	lvl.SetCurrentLevelID(1);
}

void GameSettings::Init()
{
	m_CurrentGameMode = GameMode::NotChosen;
	auto& input = InputManager::GetInstance();
	input.AddInputAction(InputAction{ 0, SDLK_z, TriggerType::OnPress, ControllerButton::ButtonA, new DieCommand() });
}
