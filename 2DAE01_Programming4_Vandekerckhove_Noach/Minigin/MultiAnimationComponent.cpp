#include "MiniginPCH.h"
#include "MultiAnimationComponent.h"
#include "AnimationComponent.h"
#include "CharacterComponent.h"
using namespace dae;

MultiAnimationComponent::MultiAnimationComponent(AnimState state)
	: m_State(state)
	, m_Pos()
{
}

void MultiAnimationComponent::UpdateComponent()
{
	if(m_NeedsUpdate)
	{
		for (auto& comp : m_AnimationComponents)
		{
			comp->SetOffset(m_Pos.x, m_Pos.y - comp->GetTextureFrameHeight());
		}
		m_NeedsUpdate = false;
	}

	if (m_State == AnimState::Invisible)
		return;
	
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
	if (m_State == AnimState::Invisible)
		return;
	
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

void MultiAnimationComponent::SetState(AnimState state,bool isFlipped)
{
	m_State = state;
	SetFlippedCurrent(isFlipped);
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
