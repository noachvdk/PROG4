#include "MiniginPCH.h"
#include "CharacterComponent.h"

#include "ColliderComponent.h"
#include "LevelManager.h"
#include "LivesComponent.h"
#include "SubjectComponent.h"
#include "TimeManager.h"
#include "GameSettings.h"
#include "Logger.h"
#include "PlayerComponent.h"
#include "MultiAnimationComponent.h"

using namespace dae;

CharacterComponent::CharacterComponent()
	: m_MoveDirection(direction::DownLeft)
	, m_Move(false)
	, m_Initialized(false)
	, m_OnDisc(false)
	, m_IsDead(false)
	, m_isStunned(false)
	, m_Switch(false)
	, m_StunTimer(0.0f)
	, m_MaxStunTime(2.5f)
	, m_StunSwitchTimer(0.0f)
	, m_MaxStunSwitchTime(0.5f)
	, m_MoveSpeed(75.0f)
	, m_DiscMoveSpeed(0.0f)
	, m_Distance(0.0f)
	, m_StartPos(LevelManager::GetInstance().GetGridOrigin())
	, m_CurrentPos(LevelManager::GetInstance().GetGridOrigin())
	, m_NextPos(0, 0)
	, m_CurrentCoord(0, 0)
	, m_Origin(LevelManager::GetInstance().GetGridOrigin())
	, m_DiscPos(0, 0)
	, m_DiscDelay(false)
	, m_DiscDelayTime(0)
	, m_MaxDiscDelay(1.5f)
	, m_IsFallingDown(false)
	, m_FallTimer(0)
	, m_MaxFallTime(2.0f)
	, m_FallDownDir(0, 1)
	, m_Anim(nullptr)
{
}

void CharacterComponent::move(direction dir)
{
	if (m_Move || m_OnDisc || m_isStunned || m_IsFallingDown)
		return;

	auto& levelmanager = LevelManager::GetInstance();
	const float fallDownOffset{ 0.3f };
	m_Move = true;
	m_MoveDirection = dir;

	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);
	//std::cout << currentHexCoord.x << " and " << currentHexCoord.y << "\n";
	//std::cout << m_CurrentPos.x << " and " << m_CurrentPos.y << "\n";

	if (m_MoveDirection == direction::UpLeft)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingAway, false);
		currentHexCoord.x -= 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
		m_FallDownDir.x = -fallDownOffset;
		m_FallDownDir.y = -fallDownOffset;
	}
	else if (m_MoveDirection == direction::UpRight)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingAway, true);
		currentHexCoord.x -= 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
		m_FallDownDir.x = fallDownOffset;
		m_FallDownDir.y = -fallDownOffset;
	}
	else if (m_MoveDirection == direction::DownLeft)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingForward, false);
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
		m_FallDownDir.x = -fallDownOffset;
	}
	else if (m_MoveDirection == direction::DownRight)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingForward, true);
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
		m_FallDownDir.x = fallDownOffset;
	}
	const auto subject = GetParentObject()->GetComponent<SubjectComponent>();
	//move to hex/Disc or fall off
	if (levelmanager.GetIsHexValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetHexPosByCoord(currentHexCoord);
	}
	else if(levelmanager.GetIsDiscValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetDiscPosByCoord(currentHexCoord);
		m_DiscPos = m_NextPos;
		m_OnDisc = true;
		m_DiscMoveSpeed = levelmanager.GetDiscMoveSpeed();
		m_CurrentCoord = currentHexCoord;
	}
	else
	{
		m_IsFallingDown = true;
		if (subject) subject->Notify(Event::ActorFell);
		auto lives = GetParentObject()->GetComponent<LivesComponent>();
		if (lives) lives->DecreaseHealth();
		auto collider = GetParentObject()->GetComponent<ColliderComponent>();
		if (collider) collider->SetDisabled();
	}

	//std::cout << currentHexCoord.x << " and " << currentHexCoord.y << "\n";
	//std::cout << m_NextPos.x << " and " << m_NextPos.y << "\n";
}

void CharacterComponent::SetAnimComponent(MultiAnimationComponent* anim)
{
	m_Anim = anim;
}

void CharacterComponent::UpdateComponent()
{
	//when the scene loads this will do it once
	if(!m_Initialized)
	{
		if(GameSettings::GetInstance().GetGameMode() == GameMode::Coop)
		{
			const auto player = GetParentObject()->GetComponent<PlayerComponent>();
			if (player)
			{
				const int id = player->GetPlayerID();
				const int steps = LevelManager::GetInstance().GetAmountOfSteps();
				if (id == 0)
				{
					int yOffset = (steps + 1) / 2;
					m_StartPos = LevelManager::GetInstance().GetHexPosByCoord(glm::vec2(steps, -yOffset));
				}
				else if (id == 1)
				{
					int yOffset = ((steps + 1) / 2)-1;
					if (steps % 2 == 0)
						yOffset++;
					m_StartPos = LevelManager::GetInstance().GetHexPosByCoord(glm::vec2(steps, yOffset));
				}
			}
		}
		m_CurrentPos = m_StartPos;
		m_Initialized = true;
		if(m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}

	if (m_IsDead)
		return;

	if (m_IsFallingDown)
	{
		m_FallTimer += TimeManager::GetInstance().GetDeltaTime();
		if (m_FallTimer >= m_MaxFallTime)
		{
			m_FallTimer = 0;
			m_IsFallingDown = false;
			auto collider = GetParentObject()->GetComponent<ColliderComponent>();
			if (collider) collider->SetEnabled();
			Die();
			return;
		}
		if (m_FallDownDir.y != 1 && m_FallTimer >= 1) //jump up then fall down
			m_FallDownDir.y = 1;
		
		m_CurrentPos += (m_FallDownDir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed * 2.0f);
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
		return;
	}

	auto& levelmanager = LevelManager::GetInstance();

	//Flash anim when hit
	if(m_isStunned)
	{
		m_StunTimer += TimeManager::GetInstance().GetDeltaTime();
		m_StunSwitchTimer += TimeManager::GetInstance().GetDeltaTime();

		if(m_StunSwitchTimer >= m_MaxStunSwitchTime)
		{
			if(!m_Switch)
			{
				if (m_Anim)
					m_Anim->SetAnimState(AnimState::FacingForward);
			}
			else
			{
				if (m_Anim)
					m_Anim->SetAnimState(AnimState::Invisible);
			}
			m_StunSwitchTimer = 0.0f;
			m_Switch = !m_Switch;
		}
		
		if(m_StunTimer >= m_MaxStunTime)
		{
			m_StunTimer = 0;
			m_StunSwitchTimer = 0.0f;
			m_isStunned = false;
			m_Switch = false;
			if (m_Anim)
				m_Anim->SetAnimState(AnimState::FacingForward);
			auto collider = GetParentObject()->GetComponent<ColliderComponent>();
			if (collider) collider->SetEnabled();
			return;
		}
		return;
	}
	
	//move pos if needed
	if(m_Move && !m_OnDisc)
	{
		//update currentpos to nextpos using deltatime and a dir vector

		auto dir = m_NextPos - m_CurrentPos;
		dir = glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);
		
		if (glm::distance(m_CurrentPos, m_NextPos) <= 1.0f)
		{
			m_Move = false;
			m_CurrentPos = m_NextPos;
			if (!levelmanager.GetIsHexFlippedByPos(m_NextPos))
			{
				const auto subject = GetParentObject()->GetComponent<SubjectComponent>();
				if (subject)
					subject->Notify(Event::ColorChange);
				levelmanager.ChangeHexColorByPos(m_NextPos);
			}
		}
		
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}
	else if(m_Move && m_OnDisc)
	{
		auto dir = m_NextPos - m_CurrentPos;
		dir = glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);

		if (glm::distance(m_CurrentPos, m_NextPos) <= 1.0f)
			m_DiscDelay = true;
		else
			m_DiscDelay = false;
		
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}
	else if(m_OnDisc)
	{
		const glm::vec2 topPos = LevelManager::GetInstance().GetDiscTopPos();
		auto dir = topPos - m_CurrentPos;
		dir = glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_DiscMoveSpeed);

		if (glm::distance(m_CurrentPos, topPos) <= 1.0f)
		{
			m_Move = false;
			m_OnDisc = false;
			m_CurrentPos = m_Origin;
		}
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}

	if(m_DiscDelay)
	{
		m_DiscDelayTime += TimeManager::GetInstance().GetDeltaTime();

		if (m_DiscDelayTime >= m_MaxDiscDelay)
		{
			m_DiscDelay = false;
			m_Move = false;
			LevelManager::GetInstance().SetDiscSteppedOn(m_CurrentCoord);
			m_DiscDelayTime = 0.0f;
		}
	}
}

void CharacterComponent::PostAddedToGameObject()
{
}

void CharacterComponent::Notify(Event event)
{
	if (event == Event::LevelFinished || event == Event::ActorDied)
		Die();
	else if (event == Event::ActorHitPurple)
		CollisionWithPurpleEnemy();
	else if (event == Event::CatchedSlickOrSam)
		CollisionWithGreenEnemy();
}

void CharacterComponent::Die()
{
	m_CurrentPos = m_StartPos;
	m_NextPos = m_CurrentPos;
	m_Move = false;
	m_isStunned = false;
	m_IsFallingDown = false;
	m_FallTimer = 0;
	m_StunSwitchTimer = 0;
	m_StunTimer = 0;
	if (m_Anim)
		m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
}

void CharacterComponent::CollisionWithPurpleEnemy()
{
	m_isStunned = true;
	m_CurrentPos = m_NextPos;
	m_Move = false;
	if (m_Anim)
	{
		m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
		m_Anim->SetAnimState(AnimState::Invisible);		
	}
	Logger::GetInstance().Log(LogType::Info, "Hit purple enemy");
	auto collider = GetParentObject()->GetComponent<ColliderComponent>();
	if (collider) collider->SetDisabled();
}

void CharacterComponent::CollisionWithGreenEnemy() const
{
	Logger::GetInstance().Log(LogType::Info, "Hit green enemy");
}
