#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "AllComponents.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	auto background = new TextureComponent{ "background.jpg" };
	go->AddComponent(background);
	auto howTo1 = new FontComponent{ "Lingua.otf", 18,"P1 : z to die, q/s/d add points" };
	howTo1->AddOffset(200, 250);
	auto howTo2 = new FontComponent{ "Lingua.otf", 18,"P2 : (keypad) 3 to die, 1/2/3 add points" };
	howTo2->AddOffset(200, 270);
	auto howToController = new FontComponent{ "Lingua.otf", 18,"Controller : A to die, X/Y/B add points" };
	howToController->AddOffset(200, 290);
	go->AddComponent(howTo1);
	go->AddComponent(howTo2);
	go->AddComponent(howToController);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->SetPosition(216, 180);
	auto logo = new TextureComponent{ "logo.png" };
	go->AddComponent(logo);
	auto fps = new FpsComponent{ "lingua.otf",24 };
	go->AddComponent(fps);
	scene.Add(go);

	auto to = std::make_shared<GameObject>();
	auto title = new FontComponent{ "Lingua.otf", 36,"Programming 4 Assignment" };
	title->AddOffset(80, 20);
	to->AddComponent(title);
	scene.Add(to);

	auto PlayerOneObject = std::make_shared<GameObject>();
	auto PlayerOne = new PlayerComponent{ 0 };
	PlayerOneObject->AddComponent(PlayerOne);
	auto livesOne = new LivesComponent{ 3,"Lingua.otf",16 };
	PlayerOneObject->AddComponent(livesOne);
	auto PointOne = new PointComponent{"Lingua.otf",16 };
	PlayerOneObject->AddComponent(PointOne);
	PointOne->AddTextOffset(550, 0);
	scene.Add(PlayerOneObject);
	PlayerOneObject->SetPosition(10, 75);
	

	auto PlayerTwoObject = std::make_shared<GameObject>();
	auto PlayerTwo = new PlayerComponent{ 1 };
	PlayerTwoObject->AddComponent(PlayerTwo);
	auto livesTwo = new LivesComponent{ 3,"Lingua.otf",16 };
	PlayerTwoObject->AddComponent(livesTwo);
	PlayerTwoObject->SetPosition(10, 100);
	auto PointTwo = new PointComponent{ "Lingua.otf",16 };
	PlayerTwoObject->AddComponent(PointTwo);
	PointTwo->AddTextOffset(550, 0);
	scene.Add(PlayerTwoObject);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	//in the properties of project go to Linker>System and set SubSystem to Console for console window
	// Windows was the default
	
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		input.AddInputAction(InputAction{ 0, SDLK_z, TriggerType::OnPress, ControllerButton::ButtonA, new DieCommand() });
		input.AddInputAction(InputAction{ 1, SDLK_KP_5, TriggerType::OnPress, ControllerButton::ButtonA, new DieCommand() });
		input.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonB, new DefeatedCoilyCommand() });
		input.AddInputAction(InputAction{ 1, SDLK_KP_1, TriggerType::OnPress, ControllerButton::ButtonB, new DefeatedCoilyCommand() });
		input.AddInputAction(InputAction{ 0, SDLK_s, TriggerType::OnPress, ControllerButton::ButtonY, new ColorChangeCommand() });
		input.AddInputAction(InputAction{ 1, SDLK_KP_2, TriggerType::OnPress, ControllerButton::ButtonY, new ColorChangeCommand() });
		input.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonX, new CatchSlickOrSamCommand() });
		input.AddInputAction(InputAction{ 1, SDLK_KP_3, TriggerType::OnPress, ControllerButton::ButtonX, new CatchSlickOrSamCommand() });
		
		auto& time = TimeManager::GetInstance();
		bool doContinue = true;
		auto lastTime = high_resolution_clock::now();
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			const float deltaTime = duration<float>(currentTime - lastTime).count();
			time.Update(deltaTime);

			doContinue = input.ProcessInput();

			sceneManager.Update();
			renderer.Render();
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}
