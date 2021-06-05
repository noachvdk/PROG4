#pragma once
#include "GameObject.h"
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


