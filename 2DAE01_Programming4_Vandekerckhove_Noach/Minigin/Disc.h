#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
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
	bool GetIsSteppedOn()const { return m_SteppedOn; }
	bool GetIsActive()const { return m_Active; }
	float GetDiscSpeed() const { return m_MoveSpeed; }
	glm::vec2 GetPos() const { return m_StartPos; }
	glm::vec2 GetCoord() const { return m_Coord; }
	glm::vec2 GetOrigin() const { return m_Origin; }

	void SetActive() { m_Active = true; }
    void SetSteppedOn() { m_SteppedOn = true; }

private:
	void SetRandomCoord(int max);
	void CalcPos();
	
	bool m_Used;
	bool m_SteppedOn;
	bool m_Side;
	bool m_Active;
	const int m_Radius;
	const float m_MoveSpeed;
	glm::vec2 m_Coord;
	glm::vec2 m_StartCoord;
	glm::vec2 m_Pos;
	glm::vec2 m_StartPos;
	glm::vec2 m_Origin; //with offset to make disc go above the pyramid
	const glm::vec2 m_StartOrigin; //origin without offset 
	std::shared_ptr<dae::AnimationComponent> m_Anim;
};

