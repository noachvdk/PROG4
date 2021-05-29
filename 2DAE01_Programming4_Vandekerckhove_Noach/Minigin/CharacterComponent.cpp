#include "MiniginPCH.h"
#include "CharacterComponent.h"
#include "TextureComponent.h"
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
	,m_Switch(false)
	, m_StunTimer(0.0f)
	, m_MaxStunTime(2.5f)
	, m_StunSwitchTimer(0.0f)
	, m_MaxStunSwitchTime(0.5f)
	, m_MoveSpeed(5.0f)
	, m_Distance(0.0f)
	, m_StartPos(LevelManager::GetInstance().GetGridOrigin())
	, m_CurrentPos(LevelManager::GetInstance().GetGridOrigin())
	, m_Origin(LevelManager::GetInstance().GetGridOrigin())
	, m_NextPos(0, 0)
	, m_Anim(nullptr)
{
}

void CharacterComponent::move(direction dir)
{
	if (m_Move || m_OnDisc || m_isStunned)
		return;

	auto& levelmanager = LevelManager::GetInstance();
	m_Move = true;
	m_MoveDirection = dir;

	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);
	levelmanager.SetPlayerStandingOnHex(m_CurrentPos, false);
	levelmanager.SetPlayerStandingOnHex(m_CurrentPos, nullptr);
	//std::cout << currentHexCoord.x << " and " << currentHexCoord.y << "\n";
	//std::cout << m_CurrentPos.x << " and " << m_CurrentPos.y << "\n";

	if (m_MoveDirection == direction::UpLeft)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingAway, false);
		currentHexCoord.x -= 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
	}
	else if (m_MoveDirection == direction::UpRight)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingAway, true);
		currentHexCoord.x -= 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
	}
	else if (m_MoveDirection == direction::DownLeft)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingForward, false);
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
	}
	else if (m_MoveDirection == direction::DownRight)
	{
		if (m_Anim)
			m_Anim->SetState(AnimState::FacingForward, true);
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
	}

	const auto subject = GetParentObject()->GetComponent<SubjectComponent>();	
	//move to hex/Disc or fall off
	if (levelmanager.GetIsHexValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetHexPosByCoord(currentHexCoord);
		if (!levelmanager.GetIsHexFlippedByCoord(currentHexCoord))
		{	
			if (subject)
				subject->Notify(Event::ColorChange);
		}
		levelmanager.ChangeHexColorByPos(m_NextPos);
	}
	else if(levelmanager.GetIsDiscValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetDiscPosByCoord(currentHexCoord);
		m_OnDisc = true;
		m_CurrentCoord = currentHexCoord;
	}
	else
	{
		if (subject)
			subject->Notify(Event::ActorFell);
		m_NextPos = m_StartPos;
		m_CurrentPos = m_StartPos;
		auto lives = GetParentObject()->GetComponent<LivesComponent>();
		if (lives)
			lives->DecreaseHealth(1);
	}

	m_MiddlePos = (m_CurrentPos + m_NextPos);
	m_MiddlePos.x /= 2;
	m_MiddlePos.y /= 2;
	m_Distance = glm::distance(m_CurrentPos, m_NextPos);
	//std::cout << currentHexCoord.x << " and " << currentHexCoord.y << "\n";
	//std::cout << m_NextPos.x << " and " << m_NextPos.y << "\n";
	//
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
			return;
		}
		return;
	}

	
	//move pos if needed
	if(m_Move && !m_OnDisc)
	{
		//update currentpos to nextpos using deltatime and a dir vector

		auto dir = m_NextPos - m_CurrentPos;
		glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);

		if (glm::distance(m_CurrentPos, m_NextPos) <= m_Distance / 2)
		{
			LevelManager::GetInstance().SetPlayerStandingOnHex(m_NextPos, true);
			LevelManager::GetInstance().SetPlayerStandingOnHex(m_NextPos, this);
		}
		
		if (glm::distance(m_CurrentPos, m_NextPos) <= 1.0f)
		{
			m_Move = false;
			m_CurrentPos = m_NextPos;
			
		}

		m_pParentObj->GetTransform().SetPosition(m_CurrentPos.x, m_CurrentPos.y, 1.0f);
		
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}
	else if(m_Move && m_OnDisc)
	{
		auto dir = m_NextPos - m_CurrentPos;
		glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);

		if (glm::distance(m_CurrentPos, m_NextPos) <= 1.0f)
		{
			m_Move = false;
			m_CurrentPos = m_NextPos;
			LevelManager::GetInstance().SetDiscSteppedOn(m_CurrentCoord);
		}
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}
	else if(m_OnDisc)
	{
		const glm::vec2 topPos = LevelManager::GetInstance().GetDiscTopPos();
		auto dir = topPos - m_CurrentPos;
		glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);

		if (glm::distance(m_CurrentPos, topPos) <= 1.0f)
		{
			m_Move = false;
			m_OnDisc = false;
			m_CurrentPos = m_Origin;
		}
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}
}

void CharacterComponent::PostAddedToGameObject()
{
}

void CharacterComponent::Notify(Event event)
{
	if (event == Event::LevelFinished)
	{
		m_CurrentPos = m_StartPos;
		m_NextPos = m_CurrentPos;
		m_Move = false;
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}
	else if (event == Event::ActorDied)
	{
		m_CurrentPos = m_StartPos;
		m_NextPos = m_CurrentPos;
		m_Move = false;
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}
	else if (event == Event::ActorHitPurple)
		CollisionWithPurpleEnemy();
	else if (event == Event::ActorHitGreen)
		CollisionWithGreenEnemy();
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
	//const auto subject = m_pParentObj->GetComponent<SubjectComponent>();
	//if (subject) subject->Notify(Event::ActorHitPurple);
	Logger::GetInstance().Log(LogType::Info, "Hit purple enemy");
}

void CharacterComponent::CollisionWithGreenEnemy() const
{
	//const auto subject = m_pParentObj->GetComponent<SubjectComponent>();
	//if (subject) subject->Notify(Event::ActorHitGreen);
	Logger::GetInstance().Log(LogType::Info, "Hit green enemy");
}
