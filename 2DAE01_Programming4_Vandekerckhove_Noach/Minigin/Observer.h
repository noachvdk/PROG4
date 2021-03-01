#pragma once

namespace dae
{
	enum class Event
	{
		ActorDied,
		ActorTakeDamage,
		ColorChange,
		DefeatedCoily,
		CatchedSlickOrSam
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event) = 0;
	};
}


