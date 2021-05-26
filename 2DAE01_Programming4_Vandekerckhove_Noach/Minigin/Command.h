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

class MoveUpLeft : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			character->move(direction::UpLeft);
		const auto coily = obj.GetComponent<CoilyComponent>();
		if (coily)
			coily->move(direction::UpLeft);
	}
};

class MoveUpRight : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			character->move(direction::UpRight);
		const auto coily = obj.GetComponent<CoilyComponent>();
		if (coily)
			coily->move(direction::UpRight);
	}
};

class MoveDownLeft : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			character->move(direction::DownLeft);
		const auto coily = obj.GetComponent<CoilyComponent>();
		if (coily)
			coily->move(direction::DownLeft);
	}
};

class MoveDownRight : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto character = obj.GetComponent<CharacterComponent>();
		if (character)
			character->move(direction::DownRight);
		const auto coily = obj.GetComponent<CoilyComponent>();
		if (coily)
			coily->move(direction::DownRight);
	}
};