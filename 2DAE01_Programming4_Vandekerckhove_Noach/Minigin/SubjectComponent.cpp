#include "MiniginPCH.h"
#include "SubjectComponent.h"

using namespace dae;

SubjectComponent::SubjectComponent()
	: m_Subject(std::make_unique<Subject>())
{
}

void SubjectComponent::UpdateComponent()
{
}

void dae::SubjectComponent::PostAddedToGameObject()
{
	for(auto pComp : GetParentObject()->GetAllComponents())
	{
		auto obv = dynamic_cast<Observer*>(pComp);
		if (obv)
			AddObserver(obv);
	}
}

void SubjectComponent::Notify(Event event) const
{
	m_Subject->Notify(event);
}

void SubjectComponent::AddObserver(Observer* observer)
{
	m_Subject->AddObserver(observer);
}
