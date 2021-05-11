#include "MiniginPCH.h"
#include "Disc.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Disc::Disc(const glm::vec2& coordpos, const glm::vec2& originPos, const int rad)
	: used(false)
	, steppedOn(false)
	, coord(coordpos)
	, startCoord(coordpos)
	, pos(0, 0)
	, origin(originPos)
{
	const glm::vec2 offset{ (glm::sin(60 * M_PI / 180) * 2) * rad, rad * 1.5f };
	glm::vec2 basePos{ origin.x + (offset.x * coordpos.y) , origin.y + (offset.y * coordpos.x) };
	if ((int(coordpos.x) % 2) != 0)
		basePos.x += offset.x / 2; //push odd rows by half the offset to avoid overlapping
	origin.y -= (offset.y);
	pos = basePos;
	startPos = basePos;

	anim = std::make_shared<dae::AnimationComponent>("DiscAnim.png", 1, 4, 0.5f, dae::AnimState::Idle, true);
	anim->SetOffset(pos.x, pos.y);
}

void Disc::Reset()
{
	used = false;
	coord = startCoord;
	steppedOn = false;
	pos = startPos;
}

void Disc::Update()
{
	if (steppedOn)
	{
		auto dir = origin - pos;
		glm::normalize(dir);
		
		pos += (dir * dae::TimeManager::GetInstance().GetDeltaTime() * 5.0f);
		if (glm::distance(pos, origin) <= 1.0f)
		{
			steppedOn = false;
			pos = origin;
			coord = glm::vec2(INFINITE, INFINITE);
			used = true;
        }
	}
	anim->SetOffset(pos.x, pos.y);
	anim->UpdateComponent();
}

void Disc::Render()
{
	anim->RenderComponent();
}
