#include "MiniginPCH.h"
#include "QbertGame.h"
#include "VisualBenchmarking.h"
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

	//MainMenu
	{
		auto& MainMenuScene = SceneManager::GetInstance().CreateScene("MainMenu");
		auto go = std::make_shared<GameObject>();
		const auto background = new TextureComponent{ "background.jpg" };
		go->AddComponent(background);
		auto howTo1 = new FontComponent{ "Cooper.ttf", 18,"P1 : AEQD to move(azerty)" };
		howTo1->AddOffset(150, 250);
		auto howTo2 = new FontComponent{ "Cooper.ttf", 18,"P2 : (keypad) 3 to die, 1/2/3 add points" };
		howTo2->AddOffset(150, 270);
		auto howToController = new FontComponent{ "Cooper.ttf", 18,"Controller : A/X/Y/B to move" };
		howToController->AddOffset(150, 290);
		auto howToBasics = new FontComponent{ "Cooper.ttf", 18,"Escape(start) to quit and P(back) to (un)mute music" };
		howToBasics->AddOffset(150, 310);
		auto menu = new MainMenuUIComponent{};
		menu->AddOption("SinglePlayer",GameMode::SinglePlayer);
		menu->AddOption("Coop", GameMode::Coop);
		menu->AddOption("Versus", GameMode::Versus);
		go->AddComponent(menu);
		go->AddComponent(howTo1);
		go->AddComponent(howTo2);
		go->AddComponent(howToController);
		go->AddComponent(howToBasics);
		
		MainMenuScene.Add(go);

		//fps
		auto fpsgo = std::make_shared<GameObject>();
		fpsgo->AddComponent(new FpsComponent{ "lingua.otf",24 });
		MainMenuScene.Add(fpsgo);
	}

	//singlePlayerLevel
	{
		auto& LevelScene = SceneManager::GetInstance().CreateScene("SinglePlayer");
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
		const auto pointComp = new PointComponent{ "Cooper.ttf",16 };
		pointComp->AddTextOffset(550, 75);
		const auto characterComp = new CharacterComponent();
		auto playerCollider = new ColliderComponent(ColliderLayer::Friendly);
		const auto liveComp = new LivesComponent{ 3,"Cooper.ttf",16,true };
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
		multiUggAnim->SetScale(1.2f);
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
		multiWrongwayAnim->SetScale(1.2f);
		wrongway->SetAnimComponent(multiWrongwayAnim);
		wrongwayCollider->SetAnimComponent(multiWrongwayAnim);
		WrongwayGO->AddComponent(wrongway);
		WrongwayGO->AddComponent(multiWrongwayAnim);
		WrongwayGO->AddComponent(wrongwayCollider);
		Levelsubject->AddObserver(wrongway);
		Player01subject->AddObserver(wrongway);
		LevelScene.Add(WrongwayGO);

		//InputActions
		LevelScene.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonY, std::make_shared<MoveUpLeft>() });
		LevelScene.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, std::make_shared<MoveUpRight>() });
		LevelScene.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonX, std::make_shared<MoveDownLeft>() });
		LevelScene.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonA, std::make_shared<MoveDownRight>() });
	}

	//CoopLevel
	{
		auto& CoopLevelScene = SceneManager::GetInstance().CreateScene("Coop");
		//subjects
		auto Player01subject = new SubjectComponent{};
		auto Player02subject = new SubjectComponent{};
		auto Levelsubject = new SubjectComponent{};

		//Level
		auto LevelObj = std::make_shared<GameObject>();
		const auto level = new LevelComponent();
		LevelObj->AddComponent(level);
		LevelObj->AddComponent(Levelsubject);
		CoopLevelScene.Add(LevelObj);
		Player01subject->AddObserver(level);
		Player02subject->AddObserver(level);

		//fps
		auto go = std::make_shared<GameObject>();
		go->AddComponent(new FpsComponent{ "lingua.otf",24 });
		CoopLevelScene.Add(go);

		//Player 1
		const auto characterComp01 = new CharacterComponent();
		{
			auto Player01Object = std::make_shared<GameObject>();
			const auto pointComp01 = new PointComponent{ "Cooper.ttf",16 };
			pointComp01->AddTextOffset(550, 75);
			
			auto playerCollider01 = new ColliderComponent(ColliderLayer::Friendly);
			const auto liveComp01 = new LivesComponent{ 3,"Cooper.ttf",16,true };
			liveComp01->AddTextOffset(10, 75);
			auto multiQbertAnim01 = new MultiAnimationComponent(AnimState::FacingForward);
			multiQbertAnim01->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
			multiQbertAnim01->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
			characterComp01->SetAnimComponent(multiQbertAnim01);
			playerCollider01->SetAnimComponent(multiQbertAnim01);
			Player01Object->AddComponent(multiQbertAnim01);
			Player01Object->AddComponent(Player01subject);
			Player01Object->AddComponent(liveComp01);
			Player01Object->AddComponent(characterComp01);
			Player01Object->AddComponent(pointComp01);
			Player01Object->AddComponent(playerCollider01);
			Player01Object->AddComponent(new PlayerComponent{ 0 });
			CoopLevelScene.Add(Player01Object);

			Levelsubject->AddObserver(pointComp01);
			Levelsubject->AddObserver(characterComp01);
			Levelsubject->AddObserver(liveComp01);
		}

		//Player 2
		const auto characterComp02 = new CharacterComponent();
		{
			auto Player02Object = std::make_shared<GameObject>();
			const auto pointComp02 = new PointComponent{ "Cooper.ttf",16 };
			pointComp02->AddTextOffset(550, 100);
			
			auto playerCollider02 = new ColliderComponent(ColliderLayer::Friendly);
			const auto liveComp02 = new LivesComponent{ 3,"Cooper.ttf",16,true };
			liveComp02->AddTextOffset(10, 100);
			auto multiQbertAnim02 = new MultiAnimationComponent(AnimState::FacingForward);
			multiQbertAnim02->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
			multiQbertAnim02->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
			characterComp02->SetAnimComponent(multiQbertAnim02);
			playerCollider02->SetAnimComponent(multiQbertAnim02);
			Player02Object->AddComponent(multiQbertAnim02);
			Player02Object->AddComponent(Player02subject);
			Player02Object->AddComponent(liveComp02);
			Player02Object->AddComponent(characterComp02);
			Player02Object->AddComponent(pointComp02);
			Player02Object->AddComponent(playerCollider02);
			Player02Object->AddComponent(new PlayerComponent{ 1 });
			CoopLevelScene.Add(Player02Object);

			Levelsubject->AddObserver(pointComp02);
			Levelsubject->AddObserver(characterComp02);
			Levelsubject->AddObserver(liveComp02);
		}

		//Coily
		{
			auto CoilyGO = std::make_shared<GameObject>();
			auto coily = new CoilyComponent();
			auto coilyCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
			coily->AddTarget(characterComp01);
			coily->AddTarget(characterComp02);
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
			Player02subject->AddObserver(coily);
			Levelsubject->AddObserver(coily);
			CoopLevelScene.Add(CoilyGO);
		}

		//Slick and sam
		{
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
			Player02subject->AddObserver(slick);
			CoopLevelScene.Add(SlickGO);


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
			Player02subject->AddObserver(sam);
			CoopLevelScene.Add(SamGO);
		}
		//Ugg and wrongway
		{
			auto UggGO = std::make_shared<GameObject>();
			auto ugg = new UggOrWrongwayComponent(Type::Ugg);
			auto uggCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
			auto multiUggAnim = new MultiAnimationComponent(AnimState::Invisible);
			multiUggAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("UggAnim.png", 1, 4, 0.5f, AnimState::None));
			multiUggAnim->SetScale(1.2f);
			ugg->SetAnimComponent(multiUggAnim);
			uggCollider->SetAnimComponent(multiUggAnim);
			UggGO->AddComponent(ugg);
			UggGO->AddComponent(multiUggAnim);
			UggGO->AddComponent(uggCollider);
			Levelsubject->AddObserver(ugg);
			Player01subject->AddObserver(ugg);
			Player02subject->AddObserver(ugg);
			CoopLevelScene.Add(UggGO);

			auto WrongwayGO = std::make_shared<GameObject>();
			auto wrongway = new UggOrWrongwayComponent(Type::WrongWay);
			auto wrongwayCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
			auto multiWrongwayAnim = new MultiAnimationComponent(AnimState::Invisible);
			multiWrongwayAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("WrongwayAnim.png", 1, 4, 0.5f, AnimState::None));
			multiWrongwayAnim->SetScale(1.2f);
			wrongway->SetAnimComponent(multiWrongwayAnim);
			wrongwayCollider->SetAnimComponent(multiWrongwayAnim);
			WrongwayGO->AddComponent(wrongway);
			WrongwayGO->AddComponent(multiWrongwayAnim);
			WrongwayGO->AddComponent(wrongwayCollider);
			Levelsubject->AddObserver(wrongway);
			Player01subject->AddObserver(wrongway);
			Player02subject->AddObserver(wrongway);
			CoopLevelScene.Add(WrongwayGO);
		}
		//InputActions
		{
			CoopLevelScene.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonY, std::make_shared<MoveUpLeft>() });
			CoopLevelScene.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, std::make_shared<MoveUpRight>() });
			CoopLevelScene.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonX, std::make_shared<MoveDownLeft>() });
			CoopLevelScene.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonA, std::make_shared<MoveDownRight>() });
			CoopLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_7, TriggerType::OnPress, ControllerButton::ButtonY, std::make_shared<MoveUpLeft>() });
			CoopLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_9, TriggerType::OnPress, ControllerButton::ButtonB, std::make_shared<MoveUpRight>() });
			CoopLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_1, TriggerType::OnPress, ControllerButton::ButtonX, std::make_shared<MoveDownLeft>() });
			CoopLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_3, TriggerType::OnPress, ControllerButton::ButtonA, std::make_shared<MoveDownRight>() });
		}

	}

	//VersusLevel
	{
		auto& VersusLevelScene = SceneManager::GetInstance().CreateScene("Versus");
		//subjects
		auto Player01subject = new SubjectComponent{};
		auto Levelsubject = new SubjectComponent{};

		//Level
		auto LevelObj = std::make_shared<GameObject>();
		const auto level = new LevelComponent();
		LevelObj->AddComponent(level);
		LevelObj->AddComponent(Levelsubject);
		VersusLevelScene.Add(LevelObj);

		//fps
		auto go = std::make_shared<GameObject>();
		go->AddComponent(new FpsComponent{ "lingua.otf",24 });
		VersusLevelScene.Add(go);

		//Player 1
		const auto characterComp01 = new CharacterComponent();
		{
			auto Player01Object = std::make_shared<GameObject>();
			const auto pointComp01 = new PointComponent{ "Cooper.ttf",16 };
			pointComp01->AddTextOffset(550, 75);

			auto playerCollider01 = new ColliderComponent(ColliderLayer::Friendly);
			const auto liveComp01 = new LivesComponent{ 3,"Cooper.ttf",16,true };
			liveComp01->AddTextOffset(10, 75);
			auto multiQbertAnim01 = new MultiAnimationComponent(AnimState::FacingForward);
			multiQbertAnim01->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
			multiQbertAnim01->AddAnimationComponent(std::make_shared<AnimationComponent>("QbertFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
			characterComp01->SetAnimComponent(multiQbertAnim01);
			playerCollider01->SetAnimComponent(multiQbertAnim01);
			Player01Object->AddComponent(multiQbertAnim01);
			Player01Object->AddComponent(Player01subject);
			Player01Object->AddComponent(liveComp01);
			Player01Object->AddComponent(characterComp01);
			Player01Object->AddComponent(pointComp01);
			Player01Object->AddComponent(playerCollider01);
			Player01Object->AddComponent(new PlayerComponent{ 0 });
			VersusLevelScene.Add(Player01Object);

			Levelsubject->AddObserver(pointComp01);
			Levelsubject->AddObserver(characterComp01);
			Levelsubject->AddObserver(liveComp01);
		}

		//Coily
		{
			auto CoilyGO = std::make_shared<GameObject>();
			auto coily = new CoilyComponent(true);
			auto coilyCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
			coily->AddTarget(characterComp01);
			auto multiCoilyAnim = new MultiAnimationComponent(AnimState::None);
			multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyFacingAway.png", 1, 2, .5f, AnimState::FacingAway));
			multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyFacingForward.png", 1, 2, .5f, AnimState::FacingForward));
			multiCoilyAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("CoilyEgg.png", 1, 2, .5f, AnimState::None));
			coily->SetAnim(multiCoilyAnim);
			coilyCollider->SetAnimComponent(multiCoilyAnim);
			CoilyGO->AddComponent(coily);
			CoilyGO->AddComponent(multiCoilyAnim);
			CoilyGO->AddComponent(coilyCollider);
			CoilyGO->AddComponent(new PlayerComponent(1));

			Player01subject->AddObserver(coily);
			Levelsubject->AddObserver(coily);
			VersusLevelScene.Add(CoilyGO);
		}
		//Slick and sam
		{
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
			VersusLevelScene.Add(SlickGO);


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
			VersusLevelScene.Add(SamGO);
		}
		//Ugg and wrongway
		{
			auto UggGO = std::make_shared<GameObject>();
			auto ugg = new UggOrWrongwayComponent(Type::Ugg);
			auto uggCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
			auto multiUggAnim = new MultiAnimationComponent(AnimState::Invisible);
			multiUggAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("UggAnim.png", 1, 4, 0.5f, AnimState::None));
			multiUggAnim->SetScale(1.2f);
			ugg->SetAnimComponent(multiUggAnim);
			uggCollider->SetAnimComponent(multiUggAnim);
			UggGO->AddComponent(ugg);
			UggGO->AddComponent(multiUggAnim);
			UggGO->AddComponent(uggCollider);
			Levelsubject->AddObserver(ugg);
			Player01subject->AddObserver(ugg);
			VersusLevelScene.Add(UggGO);

			auto WrongwayGO = std::make_shared<GameObject>();
			auto wrongway = new UggOrWrongwayComponent(Type::WrongWay);
			auto wrongwayCollider = new ColliderComponent(ColliderLayer::PurpleEnemy);
			auto multiWrongwayAnim = new MultiAnimationComponent(AnimState::Invisible);
			multiWrongwayAnim->AddAnimationComponent(std::make_shared<AnimationComponent>("WrongwayAnim.png", 1, 4, 0.5f, AnimState::None));
			multiWrongwayAnim->SetScale(1.2f);
			wrongway->SetAnimComponent(multiWrongwayAnim);
			wrongwayCollider->SetAnimComponent(multiWrongwayAnim);
			WrongwayGO->AddComponent(wrongway);
			WrongwayGO->AddComponent(multiWrongwayAnim);
			WrongwayGO->AddComponent(wrongwayCollider);
			Levelsubject->AddObserver(wrongway);
			Player01subject->AddObserver(wrongway);
			VersusLevelScene.Add(WrongwayGO);
		}
		//InputActions
		{
			VersusLevelScene.AddInputAction(InputAction{ 0, SDLK_a, TriggerType::OnPress, ControllerButton::ButtonY, std::make_shared<MoveUpLeft>() });
			VersusLevelScene.AddInputAction(InputAction{ 0, SDLK_e, TriggerType::OnPress, ControllerButton::ButtonB, std::make_shared<MoveUpRight>() });
			VersusLevelScene.AddInputAction(InputAction{ 0, SDLK_q, TriggerType::OnPress, ControllerButton::ButtonX, std::make_shared<MoveDownLeft>() });
			VersusLevelScene.AddInputAction(InputAction{ 0, SDLK_d, TriggerType::OnPress, ControllerButton::ButtonA, std::make_shared<MoveDownRight>() });
			VersusLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_7, TriggerType::OnPress, ControllerButton::ButtonY, std::make_shared<MoveUpLeft>() });
			VersusLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_9, TriggerType::OnPress, ControllerButton::ButtonB, std::make_shared<MoveUpRight>() });
			VersusLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_1, TriggerType::OnPress, ControllerButton::ButtonX, std::make_shared<MoveDownLeft>() });
			VersusLevelScene.AddInputAction(InputAction{ 1, SDLK_KP_3, TriggerType::OnPress, ControllerButton::ButtonA, std::make_shared<MoveDownRight>() });
		}

	}
	
	SceneManager::GetInstance().SetCurrentSceneName("MainMenu");
}
