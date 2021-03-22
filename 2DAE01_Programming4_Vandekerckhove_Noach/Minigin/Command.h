#pragma once
#include <iostream>
#include "GameObject.h"
#include "SubjectComponent.h"
#include "LivesComponent.h"
#include "ServiceLocator.h"
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

class DuckCommand : public Command
{
public:
	void Execute(GameObject& obj)override { std::cout << "Duck"; UNREFERENCED_PARAMETER(obj);
	}
};

class FartCommand : public Command
{
public:
	void Execute(GameObject& obj)override { std::cout << "Fart"; UNREFERENCED_PARAMETER(obj);
	}
};

class FireCommand : public Command
{
public:
	void Execute(GameObject& obj)override { std::cout << "Fire"; UNREFERENCED_PARAMETER(obj);
	}
};

class DieCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto lives = obj.GetComponent<LivesComponent>();
		if (lives)
			lives->Die();
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
		obj.GetComponent<SubjectComponent>()->Notify(Event::ColorChange);
		ServiceLocator::GetSoundSystem().UnPause();
	}
};

class DefeatedCoilyCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		auto subject = obj.GetComponent<SubjectComponent>();
		if (subject)
		obj.GetComponent<SubjectComponent>()->Notify(Event::DefeatedCoily);
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