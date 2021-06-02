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

	////CoopLevel
	//{
	//	auto& LevelScene = SceneManager::GetInstance().CreateScene("CoopLevel");

	//	auto Player01subject = new SubjectComponent{};
	//	auto Player02subject = new SubjectComponent{};
	//	auto Levelsubject = new SubjectComponent{};
	//	//Level
	//	auto LevelObj = std::make_shared<GameObject>();
	//	auto level = new LevelComponent(1);
	//	LevelObj->AddComponent(level);
	//	LevelObj->AddComponent(Levelsubject);
	//	LevelScene.Add(LevelObj);
	//	//Player 1 
	//	auto PlayerOneObject = std::make_shared<GameObject>();
	//	const auto pointComp01 = new PointComponent{ "Lingua.otf",16 };
	//	const auto characterComp01 = new CharacterComponent();
	//	const auto liveComp01 = new LivesComponent{ 3,"Lingua.otf",16,true };
	//	PlayerOneObject->AddComponent(new PlayerComponent{ 0 });

	//	auto multiQbertAnim01 = new MultiAnimationComponent(AnimState::FacingForward);
	//	multiQbertAnim01->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
	//	multiQbertAnim01->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
	//	PlayerOneObject->AddComponent(multiQbertAnim01);
	//	characterComp01->SetAnimComponent(multiQbertAnim01);
	//	
	//	PlayerOneObject->SetPosition(10, 75);
	//	PlayerOneObject->AddComponent(Player01subject);
	//	PlayerOneObject->AddComponent(liveComp01);
	//	pointComp01->AddTextOffset(550, 0);
	//	PlayerOneObject->AddComponent(pointComp01);
	//	PlayerOneObject->AddComponent(characterComp01);
	//	LevelScene.Add(PlayerOneObject);

	//	Levelsubject->AddObserver(pointComp01);
	//	Levelsubject->AddObserver(characterComp01);
	//	Levelsubject->AddObserver(liveComp01);
	//	//Player 2
	//	auto PlayerTwoObject = std::make_shared<GameObject>();
	//	const auto pointComp02 = new PointComponent{ "Lingua.otf",16 };
	//	const auto characterComp02 = new CharacterComponent();
	//	const auto liveComp02 = new LivesComponent{ 3,"Lingua.otf",16,true };
	//	PlayerTwoObject->AddComponent(new PlayerComponent{ 1 });

	//	auto multiQbertAnim02 = new MultiAnimationComponent(AnimState::FacingForward);
	//	multiQbertAnim02->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
	//	multiQbertAnim02->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
	//	PlayerTwoObject->AddComponent(multiQbertAnim02);
	//	characterComp02->SetAnimComponent(multiQbertAnim02);
	//	
	//	PlayerTwoObject->SetPosition(10, 100);
	//	PlayerTwoObject->AddComponent(liveComp02);
	//	pointComp02->AddTextOffset(550, 0);
	//	PlayerTwoObject->AddComponent(pointComp02);
	//	PlayerTwoObject->AddComponent(Player02subject);
	//	PlayerTwoObject->AddComponent(characterComp02);
	//	LevelScene.Add(PlayerTwoObject);
	//	Levelsubject->AddObserver(pointComp02);
	//	Levelsubject->AddObserver(characterComp02);
	//	Levelsubject->AddObserver(liveComp02);


	//	//Coily
	//	auto CoilyGO = std::make_shared<GameObject>();
	//	//test->SetPosition(216, 180);
	//	auto coily = new CoilyComponent();
	//	coily->AddTarget(characterComp01);
	//	coily->AddTarget(characterComp02);
	//	auto multiCoilyAnim = new MultiAnimationComponent(AnimState::None);
	//	multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
	//	multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
	//	multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyEgg.png", 1, 2, .5f, AnimState::None));
	//	coily->SetAnim(multiCoilyAnim);
	//	CoilyGO->AddComponent(coily);
	//	CoilyGO->AddComponent(multiCoilyAnim);

	//	Player01subject->AddObserver(coily);
	//	Player02subject->AddObserver(coily);
	//	Levelsubject->AddObserver(coily);
	//	LevelScene.Add(CoilyGO);
	//	
	//	//fps
	//	auto go = std::make_shared<GameObject>();
	//	go->AddComponent(new FpsComponent{ "lingua.otf",24 });
	//	LevelScene.Add(go);

	//	//Slick and sam
	//	auto SlickAndSamGO = std::make_shared<GameObject>();
	//	auto sam = new SlickOrSamComponent();
	//	auto slick = new SlickOrSamComponent();
	//	auto multislickAnim = new MultiAnimationComponent(AnimState::Invisible);
	//	multislickAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("SlickAnim.png", 1, 4, 0.5f, AnimState::None));
	//	auto multiSamAnim = new MultiAnimationComponent(AnimState::Invisible);
	//	multiSamAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("SamAnim.png", 1, 4, 0.5f, AnimState::None));
	//	slick->SetAnimComponent(multiSamAnim);
	//	sam->SetAnimComponent(multislickAnim);
	//	SlickAndSamGO->AddComponent(sam);
	//	SlickAndSamGO->AddComponent(slick);
	//	SlickAndSamGO->AddComponent(multislickAnim);
	//	SlickAndSamGO->AddComponent(multiSamAnim);
	//	Levelsubject->AddObserver(slick);
	//	Levelsubject->AddObserver(sam);
	//	LevelScene.Add(SlickAndSamGO);
	//}

	//add VersusLevel
	
	SceneManager::GetInstance().SetCurrentSceneName("Demo");
	
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

