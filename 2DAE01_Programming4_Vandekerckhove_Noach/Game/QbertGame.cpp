#include "MiniginPCH.h"
#include "QbertGame.h"

#include "LevelManager.h"
#include "SceneManager.h"
#include "AllComponents.h"
#include "Scene.h"
#include "SlickOrSamComponent.h"
#include "UggOrWrongwayComponent.h"
#include "CoilyComponent.h"
#include "CharacterComponent.h"
#include "MainMenuUIComponent.h"
#include "GameSettings.h"
#include "MoveCommands.h"

using namespace dae;

void QbertGame::LoadGame() const
{
	LevelManager::GetInstance().LoadLevelWithShape("Shape.txt", "LevelSettings.txt");
	
	GameSettings::GetInstance().SetGameMode(GameMode::NotChosen);
	//MainMenu
	{
		auto& MainMenuScene = SceneManager::GetInstance().CreateScene("MainMenu");
		auto go = std::make_shared<dae::GameObject>();
		const auto background = new TextureComponent{ "background.jpg" };
		go->AddComponent(background);
		auto howTo1 = new FontComponent{ "Lingua.otf", 18,"P1 : z to die, q/s/d add points" };
		howTo1->AddOffset(200, 250);
		auto howTo2 = new FontComponent{ "Lingua.otf", 18,"P2 : (keypad) 3 to die, 1/2/3 add points" };
		howTo2->AddOffset(200, 270);
		auto howToController = new FontComponent{ "Lingua.otf", 18,"Controller : A to die, X/Y/B add points" };
		howToController->AddOffset(200, 290);
		auto menu = new MainMenuUIComponent{};
		go->AddComponent(menu);
		go->AddComponent(howTo1);
		go->AddComponent(howTo2);
		go->AddComponent(howToController);
		MainMenuScene.Add(go);

		//read both player input
		auto PlayerOneObject = std::make_shared<dae::GameObject>();
		PlayerOneObject->AddComponent(new PlayerComponent{ 0 });
		PlayerOneObject->AddComponent(new PlayerComponent{ 1 });
		MainMenuScene.Add(PlayerOneObject);

		//fps
		auto fpsgo = std::make_shared<GameObject>();
		fpsgo->AddComponent(new FpsComponent{ "lingua.otf",24 });
		MainMenuScene.Add(fpsgo);
	}

	//singlePlayerLevel
	{
		auto& LevelScene = SceneManager::GetInstance().CreateScene("SinglePlayerLevel");
		//subjects
		auto Player01subject = new SubjectComponent{};
		auto Levelsubject = new SubjectComponent{};

		//Level
		auto LevelObj = std::make_shared<GameObject>();
		const auto level = new LevelComponent();
		LevelObj->AddComponent(level);
		LevelObj->AddComponent(Levelsubject);
		LevelScene.Add(LevelObj);

		//fps
		auto go = std::make_shared<GameObject>();
		go->AddComponent(new FpsComponent{ "lingua.otf",24 });
		LevelScene.Add(go);

		//Player 1 
		auto PlayerOneObject = std::make_shared<GameObject>();
		const auto pointComp = new PointComponent{ "Lingua.otf",16 };
		pointComp->AddTextOffset(550, 75);
		const auto characterComp = new CharacterComponent();
		auto playerCollider = new ColliderComponent(ColliderLayer::Friendly);
		const auto liveComp = new LivesComponent{ 3,"Lingua.otf",16,true };
		liveComp->AddTextOffset(10, 75);
		auto multiQbertAnim = new MultiAnimationComponent(AnimState::FacingForward);
		multiQbertAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
		multiQbertAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
		characterComp->SetAnimComponent(multiQbertAnim);
		playerCollider->SetAnimComponent(multiQbertAnim);
		PlayerOneObject->AddComponent(multiQbertAnim);
		PlayerOneObject->AddComponent(Player01subject);
		PlayerOneObject->AddComponent(liveComp);
		PlayerOneObject->AddComponent(characterComp);
		PlayerOneObject->AddComponent(pointComp);
		PlayerOneObject->AddComponent(playerCollider);
		PlayerOneObject->AddComponent(new PlayerComponent{ 0 });
		LevelScene.Add(PlayerOneObject);

		Levelsubject->AddObserver(pointComp);
		Levelsubject->AddObserver(characterComp);
		Levelsubject->AddObserver(liveComp);

		//Coily
		auto CoilyGO = std::make_shared<GameObject>();
		auto coily = new CoilyComponent();
		auto coilyCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
		coily->AddTarget(characterComp);
		auto multiCoilyAnim = new MultiAnimationComponent(AnimState::None);
		multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
		multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
		multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyEgg.png", 1, 2, .5f, AnimState::None));
		coily->SetAnim(multiCoilyAnim);
		coilyCollider->SetAnimComponent(multiCoilyAnim);
		CoilyGO->AddComponent(coily);
		CoilyGO->AddComponent(multiCoilyAnim);
		CoilyGO->AddComponent(coilyCollider);

		Player01subject->AddObserver(coily);
		Levelsubject->AddObserver(coily);
		LevelScene.Add(CoilyGO);


		//Slick and sam
		auto SlickGO = std::make_shared<GameObject>();
		auto slick = new SlickOrSamComponent();
		auto slickCollider = new ColliderComponent(ColliderLayer::GreenEnemy);
		auto multislickAnim = new MultiAnimationComponent(AnimState::Invisible);
		multislickAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("SlickAnim.png", 1, 4, 0.5f, AnimState::None));
		slick->SetAnimComponent(multislickAnim);
		slickCollider->SetAnimComponent(multislickAnim);
		SlickGO->AddComponent(slick);
		SlickGO->AddComponent(multislickAnim);
		SlickGO->AddComponent(slickCollider);
		Levelsubject->AddObserver(slick);
		Player01subject->AddObserver(slick);
		LevelScene.Add(SlickGO);


		auto SamGO = std::make_shared<GameObject>();
		auto sam = new SlickOrSamComponent();
		auto samCollider = new ColliderComponent(ColliderLayer::GreenEnemy);
		auto multiSamAnim = new MultiAnimationComponent(AnimState::Invisible);
		multiSamAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("SamAnim.png", 1, 4, 0.5f, AnimState::None));
		sam->SetAnimComponent(multiSamAnim);
		samCollider->SetAnimComponent(multiSamAnim);
		SamGO->AddComponent(sam);
		SamGO->AddComponent(multiSamAnim);
		SamGO->AddComponent(samCollider);
		Levelsubject->AddObserver(sam);
		Player01subject->AddObserver(sam);
		LevelScene.Add(SamGO);

		//Ugg and wrongway
		auto UggGO = std::make_shared<GameObject>();
		auto ugg = new UggOrWrongwayComponent(Type::Ugg);
		auto uggCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
		auto multiUggAnim = new MultiAnimationComponent(AnimState::Invisible);
		multiUggAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("UggAnim.png", 1, 4, 0.5f, AnimState::None));
		ugg->SetAnimComponent(multiUggAnim);
		uggCollider->SetAnimComponent(multiUggAnim);
		UggGO->AddComponent(ugg);
		UggGO->AddComponent(multiUggAnim);
		UggGO->AddComponent(uggCollider);
		Levelsubject->AddObserver(ugg);
		Player01subject->AddObserver(ugg);
		LevelScene.Add(UggGO);

		auto WrongwayGO = std::make_shared<GameObject>();
		auto wrongway = new UggOrWrongwayComponent(Type::WrongWay);
		auto wrongwayCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
		auto multiWrongwayAnim = new MultiAnimationComponent(AnimState::Invisible);
		multiWrongwayAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("WrongwayAnim.png", 1, 4, 0.5f, AnimState::None));
		wrongway->SetAnimComponent(multiWrongwayAnim);
		wrongwayCollider->SetAnimComponent(multiWrongwayAnim);
		WrongwayGO->AddComponent(wrongway);
		WrongwayGO->AddComponent(multiWrongwayAnim);
		WrongwayGO->AddComponent(wrongwayCollider);
		Levelsubject->AddObserver(wrongway);
		Player01subject->AddObserver(wrongway);
		LevelScene.Add(WrongwayGO);

		//InputActions
		LevelScene.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonA, std::make_shared<MoveUpLeft>()});
		LevelScene.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, std::make_shared<MoveUpRight>()});
		LevelScene.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonY, std::make_shared<MoveDownLeft>()});
		LevelScene.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonX, std::make_shared<MoveDownRight>()});
	}

	//{
		////Player 1 inputs
		//input.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonA, new MoveUpLeft() });
		//input.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, new MoveUpRight() });
		//input.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonY, new MoveDownLeft() });
		//input.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonX, new MoveDownRight() });
		////Player 2 inputs
		//input.AddInputAction(InputAction{ 1, SDLK_KP_7, TriggerType::OnPress, ControllerButton::ButtonA, new MoveUpLeft() });
		//input.AddInputAction(InputAction{ 1, SDLK_KP_9, TriggerType::OnPress, ControllerButton::ButtonB, new MoveUpRight() });
		//input.AddInputAction(InputAction{ 1, SDLK_KP_1, TriggerType::OnPress, ControllerButton::ButtonY, new MoveDownLeft() });
		//input.AddInputAction(InputAction{ 1, SDLK_KP_3, TriggerType::OnPress, ControllerButton::ButtonX, new MoveDownRight() });
	//}
	
	SceneManager::GetInstance().SetCurrentSceneName("MainMenu");
}
