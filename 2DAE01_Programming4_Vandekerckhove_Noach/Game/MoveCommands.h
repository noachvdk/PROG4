#pragma once
#include "Command.h"

class MoveUpLeft : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		//const auto coily = obj.GetComponent<CoilyComponent>();
		//if (coily)
		//	coily->move(direction::UpLeft);
		const auto player = obj.GetComponentDynamic<PlayerControlledComponent>();
		if (player)
			player->move(direction::UpLeft);
	}
};

class MoveUpRight : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto player = obj.GetComponentDynamic<PlayerControlledComponent>();
		if (player)
			player->move(direction::UpRight);
	}
};

class MoveDownLeft : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto player = obj.GetComponentDynamic<PlayerControlledComponent>();
		if (player)
			player->move(direction::DownLeft);
	}
};

class MoveDownRight : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		const auto player = obj.GetComponentDynamic<PlayerControlledComponent>();
		if (player)
			player->move(direction::DownRight);
	}
};