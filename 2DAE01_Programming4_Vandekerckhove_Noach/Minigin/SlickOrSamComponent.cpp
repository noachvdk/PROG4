#include "MiniginPCH.h"
#include "SlickOrSamComponent.h"
#include "LevelManager.h"
#include "Logger.h"
#include "SubjectComponent.h"
#include "TimeManager.h"
#include "MultiAnimationComponent.h"

using namespace dae;

SlickOrSamComponent::SlickOrSamComponent()
	: m_Move(false)
	, m_Initialized(false)
	, m_IsDead(true)
	, m_MoveDirection(direction::DownLeft)
	, m_MoveSpeed(5.0f)
	, m_StartPos(LevelManager::GetInstance().GetGridOrigin())
	, m_CurrentPos(LevelManager::GetInstance().GetGridOrigin())
	, m_NextPos(0, 0)
	, m_MoveTimer(0.0f)
	, m_MaxMoveTime(2.5f)
	, m_DeathTimer(0.0f)
	, m_MaxDeathTime (float(rand() % 10 + 1))
	, m_Anim(nullptr)
{
	
}

void SlickOrSamComponent::UpdateComponent()
{
	//(re)spawning
	if (m_IsDead)
	{
		m_DeathTimer += TimeManager::GetInstance().GetDeltaTime();
		//Set target at intervals
		if (m_DeathTimer >= m_MaxDeathTime)
		{
			m_IsDead = false;
			m_CurrentPos = m_StartPos;
			if (m_Anim)
				m_Anim->SetAnimState(AnimState::None);
		}
		else
			return;
	}

	//Set target at intervals
	m_MoveTimer += TimeManager::GetInstance().GetDeltaTime();
	if (m_MoveTimer >= m_MaxMoveTime)
	{
		m_MoveDirection = RandomDirectionDown();
		MoveDown();
		m_MoveTimer = 0;
	}

	//Actually move
	if (m_Move)
	{
		//std::cout << "move towards target\n";

		auto dir = m_NextPos - m_CurrentPos;
		glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);
		if (glm::distance(m_CurrentPos, m_NextPos) <= 1.0f)
		{
			m_Move = false;
			m_CurrentPos = m_NextPos;
			LevelManager::GetInstance().ResetHexByPos(m_CurrentPos);
		}

		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}

	//if hit player
	if (LevelManager::GetInstance().GetIsHexOccupiedByPos(m_CurrentPos))
		Die();
}

void SlickOrSamComponent::Notify(Event event)
{
	if (event == Event::LevelFinished || event == Event::ActorDied || event == Event::ActorHitPurple)
	{
		Die();
	}
}

void SlickOrSamComponent::SetAnimComponent(MultiAnimationComponent* anim)
{
	m_Anim = anim;
	if (m_Anim)
	{
		m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
		m_Anim->SetAnimState(AnimState::Invisible);
	}
}

void SlickOrSamComponent::Die()
{
	m_CurrentPos = m_StartPos;
	m_NextPos = m_CurrentPos;
	m_IsDead = true;
	m_DeathTimer = 0;
	m_MoveTimer = 0;
	m_MaxDeathTime = float(rand() % 5 + 5);
	if (m_Anim)
	{
		m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
		m_Anim->SetAnimState(AnimState::Invisible);
	}
	m_Move = false;
}

direction SlickOrSamComponent::RandomDirectionDown()
{
	int random = rand() % 2 + 2;
	const auto dir = (direction)random;
	return dir;
}

void SlickOrSamComponent::MoveDown()
{
	auto& levelmanager = LevelManager::GetInstance();
	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);

	if (m_MoveDirection == direction::DownLeft)
	{
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
		if (m_Anim)
			m_Anim->SetFlippedCurrent(false);
	}
	else if (m_MoveDirection == direction::DownRight)
	{
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
		if (m_Anim)
			m_Anim->SetFlippedCurrent(true);
	}

	if (levelmanager.GetIsHexValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetHexPosByCoord(currentHexCoord);
		m_Move = true;
	}
	else
	{
		//if on last step transform
		if (currentHexCoord.x == levelmanager.GetAmountOfSteps() + 1)
			Die();
		else
			Logger::GetInstance().Log(LogType::Error, "Coily went wrong somewhere");
	}
}
