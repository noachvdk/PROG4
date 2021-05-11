#include "MiniginPCH.h"
#include "LivesComponent.h"
#include "FontComponent.h"
#include "GameSettings.h"
#include "PlayerComponent.h"
#include "SubjectComponent.h"

using namespace dae;

LivesComponent::LivesComponent(const int startingLives, const std::string& font, const unsigned int size, bool displayHealth)
	: m_CurrentLives(startingLives)
	, m_MaxLives(startingLives)
	, m_IsDead(false)
	, m_DisplayHealth(displayHealth)
	, m_NeedsUpdate(false)
{
	m_pFontComponent = new FontComponent{ font,size," " };
}

void LivesComponent::UpdateComponent()
{
	if(m_NeedsUpdate)
	{
		const auto parent = GetParentObject()->GetComponent<PlayerComponent>();
		if (parent && m_DisplayHealth)
		{
			if(m_IsDead)
			{
				m_pFontComponent->SetText("Player " + std::to_string(parent->GetPlayerID() + 1) + " has died");
			}
			else
			{
				m_pFontComponent->SetText("Player " + std::to_string(parent->GetPlayerID() + 1) + " has " + std::to_string(m_CurrentLives) + " lives");
			}
		}

	}
}

void LivesComponent::PostAddedToGameObject()
{
	GetParentObject()->AddComponent(m_pFontComponent);
	m_NeedsUpdate = true;
}

void LivesComponent::Notify(Event event)
{
	if(event == Event::ActorHealthChange)
	{
		m_NeedsUpdate = true;
	}
	else if(event == Event::ActorDied)
	{
		m_NeedsUpdate = true;
	}
	else if (event == Event::LevelFinished)
	{
		m_CurrentLives = m_MaxLives;
		m_IsDead = false;
		m_NeedsUpdate = true;
	}
}

void LivesComponent::AddTextOffset(const float x, const float y) const
{
	if (m_DisplayHealth)
		m_pFontComponent->AddOffset(x, y);
}

void LivesComponent::DecreaseHealth(int amount)
{
	m_CurrentLives -= amount;
	auto subject = GetParentObject()->GetComponent<SubjectComponent>();
	if (m_CurrentLives <= 0)
	{
		m_CurrentLives = m_MaxLives;
		if (subject)
			subject->Notify(Event::ActorDied);
		GameSettings::GetInstance().SetGameOver();
	}
	else
	{
		if (subject)
			subject->Notify(Event::ActorHealthChange);
	}
	
}
