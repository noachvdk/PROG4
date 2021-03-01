#pragma once
#include "Observer.h"

namespace dae
{
	class Subject
	{
	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

		void Notify(Event event);

	private:
		std::vector<Observer*> m_Observers;
	};
}

