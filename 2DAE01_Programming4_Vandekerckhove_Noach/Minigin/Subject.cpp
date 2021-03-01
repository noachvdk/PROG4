#include "MiniginPCH.h"
#include "Subject.h"

using namespace dae;

void Subject::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
	for (auto& obv : m_Observers)
	{
		if (observer == obv)
		{
			delete obv;
			obv = nullptr;
		}
	}
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), nullptr), m_Observers.end());
}

void Subject::Notify(Event event)
{
	for(auto& observer : m_Observers)
	{
		observer->Notify(event);
	}
}