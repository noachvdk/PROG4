#include "MiniginPCH.h"
#include "LivesComponent.h"

#include "PlayerComponent.h"

using namespace dae;

LivesComponent::LivesComponent(const int startingLives, const std::string& font, const unsigned int size)
	: m_CurrentLives(startingLives)
	, m_MaxLives(startingLives)
	, m_NeedsUpdate(true)
{
	m_pFontComponent = new FontComponent{ font,size," " };
	m_pFontComponent->SetColor(255.f, 255.f, 0.f, 255.f);
}

void LivesComponent::UpdateComponent()
{
	const auto parent = GetParentObject()->GetComponent<PlayerComponent>();
	if(m_NeedsUpdate && parent)
	{
		if (m_CurrentLives > 0)
			m_pFontComponent->SetText("P" + std::to_string(parent->GetPlayerID() + 1) + " lives : " + std::to_string(m_CurrentLives));
		else
			m_pFontComponent->SetText("P" + std::to_string(parent->GetPlayerID() + 1) + " is dead");
		m_NeedsUpdate = false;
	}

}

void LivesComponent::PostAddedToGameObject()
{
	GetParentObject()->AddComponent(m_pFontComponent);
}

void LivesComponent::Notify(Event event)
{
	switch (event)
	{
	case Event::ActorDied:
		m_CurrentLives = 0;
		m_NeedsUpdate = true;
		break;
	case Event::ActorTakeDamage:
		m_CurrentLives--;
		if (m_CurrentLives < 0)
			m_CurrentLives = 0;
		m_NeedsUpdate = true;
		break;
	default:
		break;
	}
}

void LivesComponent::AddTextOffset(const float x, const float y) const
{
	m_pFontComponent->AddOffset(x, y);
}