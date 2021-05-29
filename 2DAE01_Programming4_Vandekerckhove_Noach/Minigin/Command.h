#pragma once
#include "GameObject.h"
#include "LevelManager.h"
#include "AllComponents.h"
#include "ServiceLocator.h"
using namespace dae;

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute(GameObject& obj) = 0;
};

class NullCommand : public Command
{
public:
	void Execute(GameObject&)override {}
};

class DieCommand : public Command
{
public:
	void Execute(GameObject&)override
	{
		ServiceLocator::GetSoundSystem().Pause();
	}
};

class ColorChangeCommand : public Command
{
public:
	void Execute(GameObject& )override
	{
		ServiceLocator::GetSoundSystem().UnPause();
	}
};

class DefeatedCoilyCommand : public Command
{
public:
	void Execute(GameObject& )override
	{
		auto& ss = ServiceLocator::GetSoundSystem();
		ss.PlaySound("../Data/sounds/door1.wav", SDL_MIX_MAXVOLUME);
	}
};

class CatchSlickOrSamCommand : public Command
{
public:
	void Execute(GameObject& )override
	{
		auto& ss = ServiceLocator::GetSoundSystem();
		ss.PlayMusic("../Data/music/road.wav", SDL_MIX_MAXVOLUME);
	}
};
