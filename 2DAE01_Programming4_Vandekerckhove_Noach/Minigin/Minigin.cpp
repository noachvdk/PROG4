#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include "InputManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "AllComponents.h"
#include "VisualBenchmarking.h"
#include "audio.h"
#include "GameSettings.h"
#include "SimpleSDLSoundSystem.h"
#include "LevelManager.h"
#include "Logger.h"
#include "CollisionManager.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	_putenv("SDL_AUDIODRIVER=DirectSound");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	initAudio();
	
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
	
	srand(unsigned int(time(NULL)));
	Renderer::GetInstance().Init(m_Window);
	Session::Get().BeginSession();
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{

	//LevelManager::GetInstance().LoadLevelWithShape("Shape.txt", "LevelSettings.txt");
	//Demo scene
	{
		auto& scene = SceneManager::GetInstance().CreateScene("Demo");

		auto go = std::make_shared<GameObject>();
		auto background = new TextureComponent{ "background.jpg" };
		go->AddComponent(background);;
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
		PlayerOneObject->AddComponent(new PlayerComponent{ 0 });
		scene.Add(PlayerOneObject);
	}
}

void dae::Minigin::Cleanup()
{
	Session::Get().EndSession();
	ServiceLocator::Destroy();
	endAudio();
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
	ServiceLocator::RegisterSoundSystem(new LoggingSoundSystem( new SDLAudioSystem()));
	GameSettings::GetInstance().SetGameMode(GameMode::NotChosen);
	LoadGame();

	//in the properties of project go to Linker>System and set SubSystem to Console for console window
	// Windows was the default
	
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& collisionManager = CollisionManager::GetInstance();
		collisionManager.SetVisualize(false); //show/hide collision boxes
		
		auto& time = TimeManager::GetInstance();
		m_Continue = true;
		auto lastTime = high_resolution_clock::now();

		while(m_Continue)
		{
			const auto currentTime = high_resolution_clock::now();
			const float deltaTime = duration<float>(currentTime - lastTime).count();
			time.Update(deltaTime);
			
			m_Continue = input.ProcessInput();

			sceneManager.Update();
			collisionManager.Update();
			renderer.Render();
			
			lastTime = currentTime;
		}
	}


	Cleanup();
}

