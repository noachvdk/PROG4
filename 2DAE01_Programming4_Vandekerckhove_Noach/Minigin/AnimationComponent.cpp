#include "MiniginPCH.h"
#include "AnimationComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "TimeManager.h"
using namespace dae;

AnimationComponent::AnimationComponent(const std::string& filename, unsigned int rows, unsigned int columns, float animTime,AnimState state, bool move)
	: m_MoveIndependently(move)
	, m_IsFlipped(false)
	, m_FrameWidth(0)
	, m_FrameHeight(0)
	, m_Width(0)
	, m_Height(0)
	, m_CurrentRow(0)
	, m_CurrentColumn(0)
	, m_MaxRow(rows)
	, m_MaxColumn(columns)
	, m_OffsetX(0)
	, m_OffsetY(0)
	, m_AnimTimer(0)
	, m_MaxAnimTime(animTime)
	, m_State(state)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	SDL_QueryTexture(m_pTexture->GetSDLTexture(), nullptr, nullptr, &m_Width, &m_Height);

	m_FrameWidth = m_Width / columns;
	m_FrameHeight = m_Height / rows;

	m_SrcRect = { 0,0,m_FrameWidth,m_FrameHeight };
	m_DestRect = { 0,0,m_FrameWidth,m_FrameHeight };
}


void AnimationComponent::UpdateComponent()
{
	glm::vec3 pos{ };
	if(m_pParentObj)
	pos = m_pParentObj->GetTransform().GetPosition();

	m_DestRect.x = static_cast<int>(pos.x) - (m_FrameWidth/2) + int(m_OffsetX);
	m_DestRect.y = static_cast<int>(pos.y) - (m_FrameHeight / 2) + int(m_OffsetY);
	m_AnimTimer += TimeManager::GetInstance().GetDeltaTime();

	if(m_AnimTimer >= m_MaxAnimTime)
	{
		m_CurrentColumn++;
		if (m_CurrentColumn >= m_MaxColumn)
		{
			m_CurrentColumn = 0;
			++m_CurrentRow;
			if(m_CurrentRow >= m_MaxRow)
			{
				m_CurrentRow = 0;
			}
		}
			
		m_AnimTimer = 0.0f;
	}

	m_SrcRect = { m_CurrentColumn * m_FrameWidth, m_CurrentRow * m_FrameHeight, m_FrameWidth, m_FrameHeight };
}

void AnimationComponent::RenderComponent()
{
	Renderer::GetInstance().RenderTexture(*m_pTexture, m_DestRect, m_SrcRect, m_IsFlipped);
	//Renderer::GetInstance().RenderSquare(m_DestRect);
}

void AnimationComponent::SetOffset(float x, float y)
{
	m_OffsetX = x;
	m_OffsetY = y;
}
