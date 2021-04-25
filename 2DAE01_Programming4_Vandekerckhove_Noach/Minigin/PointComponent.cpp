#include "MiniginPCH.h"
#include "PointComponent.h"

#include "PlayerComponent.h"

using namespace dae;

PointComponent::PointComponent(const std::string& font, const unsigned int size)
	: m_CurrentPoints()
	, m_NeedsUpdate(true)
{
	m_pFontComponent = new FontComponent{ font,size," " };
	m_pFontComponent->SetColor(255.f, 255.f, 0.f, 255.f);
}

void PointComponent::UpdateComponent()
{
	const auto player = GetParentObject()->GetComponent<PlayerComponent>();
	if(m_NeedsUpdate && player)
	{
		m_pFontComponent->SetText("P" + std::to_string(player->GetPlayerID() + 1) + " : " +std::to_string(m_CurrentPoints));
		m_NeedsUpdate = false;
	}
}

void PointComponent::PostAddedToGameObject()
{
	GetParentObject()->AddComponent(m_pFontComponent);
}

void PointComponent::AddTextOffset(const float x, const float y) const
{
	m_pFontComponent->AddOffset(x, y);
}

void PointComponent::Notify(Event event)
{
	switch(event)
	{
	case Event::ColorChange:
		AddPoints(25);
		break;
	case Event::CatchedSlickOrSam:
		AddPoints(300);
		break;
	case Event::DefeatedCoily:
		AddPoints(500);
		break;
	case Event::LevelFinished:
		ResetPoints();
		break;
	default:
		break;
	}

	m_NeedsUpdate = true;
}
