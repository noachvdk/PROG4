#pragma once
#include <iostream>
#include "GameObject.h"
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
		obj.Notify(Event::ActorDied);
	}
};

class TakeDamageCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		obj.Notify(Event::ActorTakeDamage);
	}
};

class ColorChangeCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		obj.Notify(Event::ColorChange);
	}
};

class DefeatedCoilyCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		obj.Notify(Event::DefeatedCoily);
	}
};

class CatchSlickOrSamCommand : public Command
{
public:
	void Execute(GameObject& obj)override
	{
		obj.Notify(Event::CatchedSlickOrSam);
	}
};