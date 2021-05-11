#include "MiniginPCH.h"
#include "GameSettings.h"
#include "InputManager.h"
#include "AllComponents.h"
#include "SceneManager.h"
#include "Scene.h"
using namespace dae;

GameSettings::GameSettings()
{
}


void GameSettings::SetGameMode(GameMode newMode)
{
	m_CurrentGameMode = newMode;
	//auto& scene = SceneManager::GetInstance().GetCurrentScene();
	auto& input = InputManager::GetInstance();
	input.ResetInputActions();
	
	////player objs
	//if(scene.GetName() == "MainMenu")
	//{
	//	m_PlayerOneObj = std::make_shared<GameObject>();
	//	m_PlayerOneObj->AddComponent(new PlayerComponent{ 0 });

	//	m_PlayerTwoObj = std::make_shared<GameObject>();
	//	if(m_CurrentGameMode != GameMode::SinglePlayer)
	//	{
	//		m_PlayerTwoObj->AddComponent(new PlayerComponent{ 1 });
	//	}
	//}
	//else
	//{
	//	//Player 1 
	//	m_PlayerOneObj = std::make_shared<GameObject>();
	//	m_PlayerOneObj->SetPosition(10, 75);
	//	m_PlayerOneObj->AddComponent(new SubjectComponent{});
	//	m_PlayerOneObj->AddComponent(new PlayerComponent{ 0 });
	//	m_PlayerOneObj->AddComponent(new LivesComponent{ 3,true });
	//	auto Points = new PointComponent{ "Lingua.otf",16 };
	//	Points->AddTextOffset(550, 0);
	//	m_PlayerOneObj->AddComponent(Points);

	//	m_PlayerTwoObj = std::make_shared<GameObject>();
	//	if (m_CurrentGameMode != GameMode::SinglePlayer)
	//	{
	//		//Player 2
	//		m_PlayerTwoObj->SetPosition(10, 100);
	//		m_PlayerTwoObj->AddComponent(new PlayerComponent{ 1 });
	//		m_PlayerTwoObj->AddComponent(new LivesComponent{ 3,true });
	//		Points = new PointComponent{ "Lingua.otf",16 };
	//		Points->AddTextOffset(550, 0);
	//		m_PlayerTwoObj->AddComponent(Points);
	//		m_PlayerTwoObj->AddComponent(new SubjectComponent{});
	//	}

	//}

	//inputs
	if(m_CurrentGameMode == GameMode::SinglePlayer)
	{
		std::cout<<"Gamemode is Singleplayer\n";
	
		
		//Player 1 inputs
		input.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonA, new MoveUpLeft() });
		input.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, new MoveUpRight() });
		input.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonY, new MoveDownLeft() });
		input.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonX, new MoveDownRight() });
	}
	else if(m_CurrentGameMode == GameMode::Coop)
	{
		std::cout << "Gamemode is coop\n";
	
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
		std::cout << "Gamemode is Versus\n";
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

	auto& lvl = LevelManager::GetInstance();
	lvl.SetCurrentLevelID(1);
}

void GameSettings::Init()
{
	m_CurrentGameMode = GameMode::NotChosen;
	auto& input = InputManager::GetInstance();
	input.AddInputAction(InputAction{ 0, SDLK_z, TriggerType::OnPress, ControllerButton::ButtonA, new DieCommand() });
}
