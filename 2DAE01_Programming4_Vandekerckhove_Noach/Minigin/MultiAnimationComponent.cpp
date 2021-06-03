#include "MiniginPCH.h"
#include "MultiAnimationComponent.h"
#include "AnimationComponent.h"
using namespace dae;

MultiAnimationComponent::MultiAnimationComponent(AnimState state, bool movesWithParent)
	: m_NeedsUpdate(true)
	, m_MovesWithParent(movesWithParent)
	, m_Pos(0,0)
	, m_Offset(0,0)
	, m_State(state)
{
}

void MultiAnimationComponent::UpdateComponent()
{
	if(m_NeedsUpdate)
	{
		for (auto& comp : m_AnimationComponents)
		{
			if(m_MovesWithParent)
			{
				const auto pos = m_pParentObj->GetTransform().GetPosition();
				comp->SetOffset(pos.x + m_Pos.x + m_Offset.x, pos.y + m_Pos.y + m_Offset.y - comp->GetTextureFrameHeight());
			}
			else
			comp->SetOffset(m_Pos.x + m_Offset.x, m_Pos.y + m_Offset.y - comp->GetTextureFrameHeight());
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
		comp->SetOffset(pos.x + m_Offset.x, pos.y + m_Offset.y);
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

std::shared_ptr<AnimationComponent> MultiAnimationComponent::GetCurrentAnimComponent()
{
	std::shared_ptr<AnimationComponent> temp = nullptr;
	for (auto& comp : m_AnimationComponents)
	{
		if (comp->GetAnimState() == m_State)
		{
			temp = comp;
		}
	}

	return temp;
}

std::shared_ptr<AnimationComponent> MultiAnimationComponent::GetAnimComponent(AnimState state)
{
	std::shared_ptr<AnimationComponent> temp = nullptr;
	for (auto& comp : m_AnimationComponents)
	{
		if (comp->GetAnimState() == state)
		{
			temp = comp;
		}
	}

	return temp;
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

void MultiAnimationComponent::SetScale(float scale)
{
	for (auto& comp : m_AnimationComponents)
	{
		comp->SetScale(scale);
	}
}
