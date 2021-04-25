#pragma once

namespace dae
{
	enum class Event
	{
		ActorDied,
		ActorTakeDamage,
		ActorHealthChange,
		ActorFell,
		ColorChange,
		DefeatedCoily,
		CatchedSlickOrSam,
		LevelFinished
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event) = 0;
	};
}


