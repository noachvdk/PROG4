#include "MiniginPCH.h"
#include "LivesComponent.h"
#include "LivesDisplayComponent.h"
#include "PlayerComponent.h"
#include "SubjectComponent.h"

using namespace dae;

LivesComponent::LivesComponent(const int startingLives, bool DisplayHealth)
	: m_CurrentLives(startingLives)
	, m_MaxLives(startingLives)
	, m_IsDead(false)
	, m_DisplayHealth(DisplayHealth)
{
	if(DisplayHealth)
		m_pLivesDisplay = new LivesDisplayComponent{ "Lingua.otf",16 };
}

void LivesComponent::UpdateComponent()
{

}

void LivesComponent::PostAddedToGameObject()
{
	if(m_DisplayHealth)
		GetParentObject()->AddComponent(m_pLivesDisplay);
}

void LivesComponent::AddTextOffset(const float x, const float y) const
{
	if (m_DisplayHealth)
		m_pLivesDisplay->AddTextOffset(x, y);
}

void LivesComponent::DecreaseHealth(int amount)
{
	m_CurrentLives -= amount;
	if (m_CurrentLives < 0)
	{
		m_CurrentLives = 0;
		m_IsDead = true;
	}
	GetParentObject()->GetComponent<SubjectComponent>()->Notify(Event::ActorHealthChange);
}

void LivesComponent::Die()
{
	m_CurrentLives = 0;
	m_IsDead = true;
	GetParentObject()->GetComponent<SubjectComponent>()->Notify(Event::ActorHealthChange);
}
