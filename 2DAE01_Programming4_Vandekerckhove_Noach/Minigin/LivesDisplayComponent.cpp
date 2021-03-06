#include "MiniginPCH.h"
#include "LivesDisplayComponent.h"
#include "PlayerComponent.h"
#include  "FontComponent.h"
#include "LivesComponent.h"

using namespace dae;

LivesDisplayComponent::LivesDisplayComponent(const std::string& font, const unsigned int size)
{
	m_pFontComponent = new FontComponent{ font,size," " };
	m_pFontComponent->SetColor(255.f, 255.f, 0.f, 255.f);
}

void LivesDisplayComponent::UpdateComponent()
{


}

void LivesDisplayComponent::PostAddedToGameObject()
{
	GetParentObject()->AddComponent(m_pFontComponent);
	const auto parent = GetParentObject()->GetComponent<PlayerComponent>();
	const auto health = GetParentObject()->GetComponent<LivesComponent>();
	if (parent && health)
		m_pFontComponent->SetText("Player " + std::to_string(parent->GetPlayerID()+1) + " has " + std::to_string(health->GetCurrentLives()) + " lives");
}

void LivesDisplayComponent::Notify(Event event)
{
	const auto parent = GetParentObject()->GetComponent<PlayerComponent>();
	const auto health = GetParentObject()->GetComponent<LivesComponent>();

	switch (event)
	{
	case Event::ActorHealthChange:
		if (parent && health)
		{
			if(health->GetIsDead())
				m_pFontComponent->SetText("Player " + std::to_string(parent->GetPlayerID() + 1) + " has died");
			else
				m_pFontComponent->SetText("Player " + std::to_string(parent->GetPlayerID() + 1) + " has " + std::to_string(health->GetCurrentLives()) + " lives");
		}
		break;
	default:
		break;
	}
}

void LivesDisplayComponent::AddTextOffset(const float x, const float y) const
{
	m_pFontComponent->AddOffset(x, y);
}