#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
//#include <glm/glm.hpp>
#include "../3rdParty/glm/glm/glm.hpp"
#pragma warning(pop)
#include "AnimationComponent.h"
class Disc
{
public:
	Disc(const glm::vec2& originPos, const int rad, bool side);

	void Reset();
    void Update();
    void Render();
	
	bool GetIsUsed()const { return m_Used; }
	float GetDiscSpeed() const { return m_MoveSpeed; }
	glm::vec2 GetPos() const { return m_StartPos; }
	glm::vec2 GetCoord() const { return m_Coord; }
	glm::vec2 GetOrigin() const { return m_Origin; }
	
    void SetSteppedOn() { m_SteppedOn = true; }
	void SetRandomCoord(int max);

private:
	void CalcPos();
	
	bool m_Used;
	bool m_SteppedOn;
	bool m_Side;
	const int m_Radius;
	const float m_MoveSpeed;
	glm::vec2 m_Coord;
	glm::vec2 m_StartCoord;
	glm::vec2 m_Pos;
	glm::vec2 m_StartPos;
	glm::vec2 m_Origin;
	std::shared_ptr<dae::AnimationComponent> m_Anim;
};

