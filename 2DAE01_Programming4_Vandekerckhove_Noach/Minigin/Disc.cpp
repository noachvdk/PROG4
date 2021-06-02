#include "MiniginPCH.h"
#include "Disc.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "LevelManager.h"

Disc::Disc(const glm::vec2& originPos, const int rad, bool side)
	: m_Used(false)
	, m_SteppedOn(false)
	, m_Side(side)
	, m_Radius(rad)
	, m_MoveSpeed(70.0f)
	, m_Coord(0,0)
	, m_StartCoord(0,0)
	, m_Pos(0, 0)
	, m_StartPos(0,0)
	, m_Origin(originPos)
	, m_Anim(nullptr)
{
	SetRandomCoord(dae::LevelManager::GetInstance().GetAmountOfSteps());

	m_Anim = std::make_shared<dae::AnimationComponent>("DiscAnim.png", 1, 4, 0.5f, dae::AnimState::Idle, true);
	m_Anim->SetOffset(m_Pos.x, m_Pos.y);
}

void Disc::Reset()
{
	m_Used = false;
	m_Coord = m_StartCoord;
	m_SteppedOn = false;
	m_Pos = m_StartPos;
}

void Disc::Update()
{
	if (m_SteppedOn)
	{
		auto dir = m_Origin - m_Pos;
		dir = glm::normalize(dir);
		
		m_Pos += (dir * dae::TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);
		if (glm::distance(m_Pos, m_Origin) <= 1.0f)
		{
			m_SteppedOn = false;
			m_Pos = m_Origin;
			m_Coord = glm::vec2(INFINITE, INFINITE);
			m_Used = true;
        }
	}
	if(m_Anim)
	{
		m_Anim->SetOffset(m_Pos.x, m_Pos.y);
		m_Anim->UpdateComponent();
	}
	
}

void Disc::Render()
{
	if (m_Anim)m_Anim->RenderComponent();
}

void Disc::SetRandomCoord(int max)
{
	int x = rand() % (max-1) + 1;
	int y{};
	if(x % 2 == 0)
	{
		const int temp{ (x / 2) - 1 };
		if (m_Side)
			y = -x + temp;
		else
			y = x - temp;
	}
	else
	{
		const int temp{ x / 2 };
		if (m_Side)
			y = -2 - temp;
		else
			y = 1 + temp;
	}
	m_Coord = { x,y };
	m_StartCoord = m_Coord;
	CalcPos();
}

void Disc::CalcPos()
{
	const glm::vec2 offset{ (glm::sin(60 * M_PI / 180) * 2) * m_Radius, m_Radius * 1.5f };
	glm::vec2 basePos{ m_Origin.x + (offset.x * m_Coord.y) , m_Origin.y + (offset.y * m_Coord.x) };
	if ((int(m_Coord.x) % 2) != 0)
		basePos.x += offset.x / 2; //push odd rows by half the offset to avoid overlapping
	m_Origin.y -= (offset.y);
	m_Pos = basePos;
	m_StartPos = basePos;
}
