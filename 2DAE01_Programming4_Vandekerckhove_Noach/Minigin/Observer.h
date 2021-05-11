#pragma once

namespace dae
{
	enum class Event
	{
		ActorDied,
		ActorTakeDamage,
		ActorHealthChange,
		ActorFell,
		ActorMoveUpLeft,
		ActorMoveUpRight,
		ActorMoveDownLeft,
		ActorMoveDownRight,
		ColorChange,
		DefeatedCoily,
		CatchedSlickOrSam,
		LevelFinished,
		Reset
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event) = 0;
	};
}


