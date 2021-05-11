#include "MiniginPCH.h"
#include "MultiAnimationComponent.h"
#include "AnimationComponent.h"
#include "CharacterComponent.h"
using namespace dae;

MultiAnimationComponent::MultiAnimationComponent(AnimState state)
	: m_State(state)
{
}

void MultiAnimationComponent::UpdateComponent()
{
	glm::vec2 pos{};
	if (m_Character)
		pos = m_Character->GetCurrentCharacterPos();
	else
		m_Character = m_pParentObj->GetComponent<CharacterComponent>();
	
	for (auto& comp : m_AnimationComponents)
	{
		comp->SetOffset(pos.x, pos.y - comp->GetTextureFrameHeight());
	}
	
	for (auto& comp : m_AnimationComponents)
	{
		if (comp->GetAnimState() == m_State)
		{
			comp->UpdateComponent();
		}
	}
}

void MultiAnimationComponent::RenderComponent()
{
	for (auto& comp : m_AnimationComponents)
	{
		if (comp->GetAnimState() == m_State)
		{
			comp->RenderComponent();
		}
	}
}

void MultiAnimationComponent::PostAddedToGameObject()
{
	for (auto& comp : m_AnimationComponents)
	{
		glm::vec3 pos{ };
		if (m_pParentObj)
			pos = m_pParentObj->GetTransform().GetPosition();
		comp->SetOffset(pos.x, pos.y);
	}

}

void MultiAnimationComponent::Notify(Event event)
{		
	switch (event)
	{
	case Event::ActorMoveUpLeft:
		m_State = AnimState::FacingAway;
		SetFlippedCurrent(false);
		break;
	case Event::ActorMoveUpRight:
		m_State = AnimState::FacingAway;
		SetFlippedCurrent(true);
		break;
	case Event::ActorMoveDownLeft:
		m_State = AnimState::FacingForward;
		SetFlippedCurrent(false);
		break;
	case Event::ActorMoveDownRight:
		m_State = AnimState::FacingForward;
		SetFlippedCurrent(true);
		break;
	default:
		break;
	}
}

void MultiAnimationComponent::AddAnimationComponent(std::shared_ptr<AnimationComponent> comp)
{
	m_AnimationComponents.push_back(comp);
}

void MultiAnimationComponent::SetFlipped(bool value, AnimState id)
{
	for (auto& comp : m_AnimationComponents)
	{
		if (comp->GetAnimState() == id)
		{
			comp->SetFlipped(value);
		}
	}
}

void MultiAnimationComponent::SetFlippedCurrent(bool value)
{
	for (auto& comp : m_AnimationComponents)
	{
		if (comp->GetAnimState() == m_State)
		{
			comp->SetFlipped(value);
		}
	}
}
