#include "MiniginPCH.h"
#include "Hex.h"


Hex::Hex(int row, int column, const glm::vec2& origin, const int rad, int maxstep, bool can)
    : c(column)
	, r(row)
	, m_Radius(rad)
	, m_CurrentTexID(0)
    , m_MaxStep(maxstep)
	, m_CanGoBack(can)
	, m_StandingOn(false)
{
    const glm::vec2 offset{ (glm::sin(60 * M_PI / 180) * 2) * m_Radius, m_Radius * 1.5f };
    glm::vec2 basePos{ origin.x + (offset.x * c) , origin.y + (offset.y * r) };
    if ((r % 2) != 0)
        basePos.x += offset.x / 2; //push odd rows by half the offset to avoid overlapping

    m_Pos = basePos;
}

void Hex::NextTile()
{
    if (m_CanGoBack)
    {
        if (m_CurrentTexID == m_MaxStep)
        {
            m_CurrentTexID = 0;
            return;
        }
    }
    ++m_CurrentTexID;
    m_CurrentTexID = std::min(m_CurrentTexID, m_MaxStep);
}

void Hex::ResetTex()
{
    m_CurrentTexID = 0;
}

void Hex::SetNewLevel(int maxstep, bool can)
{
    m_CurrentTexID = 0;
    m_MaxStep = maxstep;
    m_CanGoBack = can;
}
