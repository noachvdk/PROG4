#pragma once
#include <iostream>
#include "GameObject.h"
#include "LevelManager.h"
#include "AllComponents.h"
#include "LivesComponent.h"
#include "ServiceLocator.h"
#include "SceneManager.h"
using namespace dae;

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute(GameObject& obj) = 0;
};

class JumpCommand : public Command
{
public:
	void Execute(GameObject& obj)override { std::cout << "jump"; UNREFERENCED_PARAMETER(obj); }
};

class DieCommand : public Command
{
public:
	void Execute(GameObject&)override
	{
		ServiceLocator::GetSoundSystem().Pause();
	}
};

class TakeDamageCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto lives = obj.GetComponent<LivesComponent>();
		if (lives)
			lives->DecreaseHealth();
	}
};

class ColorChangeCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto subject = obj.GetComponent<SubjectComponent>();
		if (subject)
		{
			subject->Notify(Event::ColorChange);
		}
		ServiceLocator::GetSoundSystem().UnPause();
	}
};

class DefeatedCoilyCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto subject = obj.GetComponent<SubjectComponent>();
		if (subject)
		{
			subject->Notify(Event::DefeatedCoily);
			subject->Notify(Event::LevelFinished);
		}
		auto& ss = ServiceLocator::GetSoundSystem();
		ss.PlaySound("../Data/sounds/door1.wav", SDL_MIX_MAXVOLUME);
	}
};

class CatchSlickOrSamCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto subject = obj.GetComponent<SubjectComponent>();
		if (subject)
		obj.GetComponent<SubjectComponent>()->Notify(Event::CatchedSlickOrSam);
		auto& ss = ServiceLocator::GetSoundSystem();
		ss.PlayMusic("../Data/music/road.wav", SDL_MIX_MAXVOLUME);
	}
};

class MoveUpLeft : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			obj.GetComponent<CharacterComponent>()->move(direction::UpLeft);
	}
};

class MoveUpRight : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			obj.GetComponent<CharacterComponent>()->move(direction::UpRight);
	}
};

class MoveDownLeft : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			obj.GetComponent<CharacterComponent>()->move(direction::DownLeft);
	}
};

class MoveDownRight : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			obj.GetComponent<CharacterComponent>()->move(direction::DownRight);
	}
};