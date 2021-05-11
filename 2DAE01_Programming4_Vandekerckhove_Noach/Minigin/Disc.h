#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
//#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "AnimationComponent.h"

#pragma warning(pop)
class Disc
{
public:
	Disc(const glm::vec2& coordpos, const glm::vec2& originPos, const int rad);

	void Reset();
    void Update();
    void Render();
	
	bool GetIsUsed()const { return used; }
	glm::vec2 GetPos() const { return pos; }
	glm::vec2 GetCoord() const { return coord; }
	glm::vec2 GetOrigin() const { return origin; }
    void SetSteppedOn() { steppedOn = true; }

private:
	bool used;
	bool steppedOn;
	glm::vec2 coord;
	glm::vec2 startCoord;
	glm::vec2 pos;
	glm::vec2 startPos;
	glm::vec2 origin;
	std::shared_ptr<dae::AnimationComponent> anim;
};

