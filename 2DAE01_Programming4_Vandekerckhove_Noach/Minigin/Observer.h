#pragma once

namespace dae
{
	enum class Event
	{
		ActorDied,
		ActorHitPurple,
		ActorHitGreen,
		ColorChange,
		DefeatedCoily,
		CatchedSlickOrSam,
		LevelFinished,
		Reset,
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event) = 0;
	};
}


