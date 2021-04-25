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

	Renderer::GetInstance().Init(m_Window);
	Session::Get().BeginSession();
	GameSettings::GetInstance().Init();
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{

	LevelManager::GetInstance().LoadLevelWithShape("Shape.txt", "LevelSettings.txt");
	
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

	//MainMenu
	{
		auto& MainMenuScene = SceneManager::GetInstance().CreateScene("MainMenu");
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
		MainMenuScene.Add(go);

		//read both player input
		auto PlayerOneObject = std::make_shared<GameObject>();
		PlayerOneObject->AddComponent(new PlayerComponent{ 0 });
		MainMenuScene.Add(PlayerOneObject);

		auto PlayerTwoObject = std::make_shared<GameObject>();
		PlayerTwoObject->AddComponent(new PlayerComponent{ 1 });
		MainMenuScene.Add(PlayerTwoObject);

		//fps
		auto fpsgo = std::make_shared<GameObject>();
		fpsgo->AddComponent(new FpsComponent{ "lingua.otf",24 });
		MainMenuScene.Add(fpsgo);
	}

	//singlePlayerLevel
	{
		auto& LevelScene = SceneManager::GetInstance().CreateScene("SinglePlayerLevel");
		auto Player01subject = new SubjectComponent{};
		auto Levelsubject = new SubjectComponent{};
		//Level
		auto LevelObj = std::make_shared<GameObject>();
		const auto level = new LevelComponent(1);
		LevelObj->AddComponent(level);
		LevelObj->AddComponent(Levelsubject);
		LevelScene.Add(LevelObj);
		Player01subject->AddObserver(level);
		//Player 1 
		auto PlayerOneObject = std::make_shared<GameObject>();
		PlayerOneObject->SetPosition(10, 75);
		const auto pointComp = new PointComponent{ "Lingua.otf",16 };
		const auto characterComp = new CharacterComponent("Qbert.png");
		const auto liveComp = new LivesComponent{ 3,"Lingua.otf",16,true };
		PlayerOneObject->AddComponent(Player01subject);
		PlayerOneObject->AddComponent(new PlayerComponent{ 0 });
		PlayerOneObject->AddComponent(liveComp);
		PlayerOneObject->AddComponent(characterComp);
		pointComp->AddTextOffset(550, 0);
		PlayerOneObject->AddComponent(pointComp);
		LevelScene.Add(PlayerOneObject);
		
		Levelsubject->AddObserver(pointComp);
		Levelsubject->AddObserver(characterComp);
		Levelsubject->AddObserver(liveComp);

		//fps
		auto go = std::make_shared<GameObject>();
		go->AddComponent(new FpsComponent{ "lingua.otf",24 });
		LevelScene.Add(go);
	}

	//CoopLevel
	{
		auto& LevelScene = SceneManager::GetInstance().CreateScene("CoopLevel");

		auto Player01subject = new SubjectComponent{};
		auto Player02subject = new SubjectComponent{};
		auto Levelsubject = new SubjectComponent{};
		//Level
		auto LevelObj = std::make_shared<GameObject>();
		auto level = new LevelComponent(1);
		LevelObj->AddComponent(level);
		LevelObj->AddComponent(Levelsubject);
		LevelScene.Add(LevelObj);
		Player01subject->AddObserver(level);
		Player02subject->AddObserver(level);
		//Player 1 
		auto PlayerOneObject = std::make_shared<GameObject>();
		const auto pointComp01 = new PointComponent{ "Lingua.otf",16 };
		const auto characterComp01 = new CharacterComponent("Qbert.png");
		const auto liveComp01 = new LivesComponent{ 3,"Lingua.otf",16,true };
		PlayerOneObject->SetPosition(10, 75);
		PlayerOneObject->AddComponent(Player01subject);
		PlayerOneObject->AddComponent(new PlayerComponent{ 0 });
		PlayerOneObject->AddComponent(liveComp01);
		pointComp01->AddTextOffset(550, 0);
		PlayerOneObject->AddComponent(pointComp01);
		PlayerOneObject->AddComponent(characterComp01);
		LevelScene.Add(PlayerOneObject);

		Levelsubject->AddObserver(pointComp01);
		Levelsubject->AddObserver(characterComp01);
		Levelsubject->AddObserver(liveComp01);
		//Player 2
		auto PlayerTwoObject = std::make_shared<GameObject>();
		const auto pointComp02 = new PointComponent{ "Lingua.otf",16 };
		const auto characterComp02 = new CharacterComponent("Qbert.png");
		const auto liveComp02 = new LivesComponent{ 3,"Lingua.otf",16,true };
		PlayerTwoObject->SetPosition(10, 100);
		PlayerTwoObject->AddComponent(new PlayerComponent{ 1 });
		PlayerTwoObject->AddComponent(liveComp02);
		pointComp02->AddTextOffset(550, 0);
		PlayerTwoObject->AddComponent(pointComp02);
		PlayerTwoObject->AddComponent(Player02subject);
		PlayerTwoObject->AddComponent(characterComp02);
		LevelScene.Add(PlayerTwoObject);
		Levelsubject->AddObserver(pointComp02);
		Levelsubject->AddObserver(characterComp02);
		Levelsubject->AddObserver(liveComp02);

		//fps
		auto go = std::make_shared<GameObject>();
		go->AddComponent(new FpsComponent{ "lingua.otf",24 });
		LevelScene.Add(go);
	}

	//add VersusLevel
	
	SceneManager::GetInstance().SetCurrentSceneName("MainMenu");
	
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
	LoadGame();

	//in the properties of project go to Linker>System and set SubSystem to Console for console window
	// Windows was the default
	
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		
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
			renderer.Render();
			
			lastTime = currentTime;


			
		}
	}


	Cleanup();
}

