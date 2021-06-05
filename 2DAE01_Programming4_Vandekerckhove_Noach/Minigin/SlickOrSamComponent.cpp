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
	, m_Catched(false)
	, m_IsDead(true)
	, m_MoveDirection(direction::DownLeft)
	, m_MoveSpeed(60.0f)
	, m_StartPos(0,0)
	, m_CurrentPos(0,0)
	, m_NextPos(0, 0)
	, m_MoveTimer(0.0f)
	, m_MaxMoveTime(2.5f)
	, m_DeathTimer(0.0f)
	, m_MaxDeathTime (float(rand() % 10 + 1))
	, m_IsFallingDown(false)
	, m_FallTimer(0)
	, m_MaxFallTime(2.0f)
	, m_FallDownDir(0,1)
	, m_Anim(nullptr)
{
	SetRandomStartPos();
	m_CurrentPos = m_StartPos;
}

void SlickOrSamComponent::UpdateComponent()
{
	if (m_Catched)
		return;
	
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

	if(m_IsFallingDown)
	{
		m_FallTimer += TimeManager::GetInstance().GetDeltaTime();
		if (m_FallTimer >= m_MaxFallTime)
		{
			m_FallTimer = 0;
			m_IsFallingDown = false;
			Die();
		}
		m_CurrentPos += (m_FallDownDir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed * 2.0f);
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
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
		auto dir = m_NextPos - m_CurrentPos;
		dir = glm::normalize(dir);

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

	////old if hit player
	//auto otherPlayer = LevelManager::GetInstance().GetPlayerOnHexByPos(m_CurrentPos);
	//if (!m_IsDead && otherPlayer && LevelManager::GetInstance().GetIsHexOccupiedByPos(m_CurrentPos))
	//{
	//	Die();
	//	otherPlayer->CollisionWithGreenEnemy();
	//}
}

void SlickOrSamComponent::Notify(Event event)
{
	if (event == Event::LevelFinished || event == Event::ActorDied || event == Event::Reset)
	{
		m_Catched = false;
		Die();
	}
	else if (event == Event::ActorHitGreen)
	{
		m_Catched = true;
		Die();
	}
	else if (event == Event::ActorHitPurple)
		Die();
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
	SetRandomStartPos();
	m_CurrentPos = m_StartPos;
	m_NextPos = m_CurrentPos;
	m_IsFallingDown = false;
	m_FallTimer = 0;
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

void SlickOrSamComponent::SetRandomStartPos()
{
	const float random = float(rand() % 2 - 1);
	auto& levelmanager = LevelManager::GetInstance();
	const glm::vec2 startCoord{ 1,random };
	if (levelmanager.GetIsHexValidByCoord(startCoord))
	{
		m_StartPos = levelmanager.GetHexPosByCoord(startCoord);
	}
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
	const float fallDownOffset{ 0.3f };
	if (m_MoveDirection == direction::DownLeft)
	{
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
		if (m_Anim)
			m_Anim->SetFlippedCurrent(false);
		m_FallDownDir.x = -fallDownOffset;
	}
	else if (m_MoveDirection == direction::DownRight)
	{
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
		if (m_Anim)
			m_Anim->SetFlippedCurrent(true);
		m_FallDownDir.x = fallDownOffset;
	}

	if (levelmanager.GetIsHexValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetHexPosByCoord(currentHexCoord);
		m_Move = true;
	}
	else
	{
		//if on last step fall of then die
		if (currentHexCoord.x == levelmanager.GetAmountOfSteps() + 1)
			m_IsFallingDown = true; 
		else
			Logger::GetInstance().Log(LogType::Error, "Slick or sam went wrong somewhere");
	}
}
