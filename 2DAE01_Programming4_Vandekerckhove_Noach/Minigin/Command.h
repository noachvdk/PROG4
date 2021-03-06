#pragma once
#include <iostream>
#include "GameObject.h"
#include "SubjectComponent.h"
#include "LivesComponent.h"
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
	}
};