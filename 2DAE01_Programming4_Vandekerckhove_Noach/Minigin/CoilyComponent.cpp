#include "MiniginPCH.h"
#include "CoilyComponent.h"
#include "LevelManager.h"
#include "Logger.h"
#include "CharacterComponent.h"
#include "ColliderComponent.h"
#include "TimeManager.h"
#include "MultiAnimationComponent.h"
#include "SubjectComponent.h"

using namespace dae;


CoilyComponent::CoilyComponent(bool playerControlled)
	: m_Move(false)
	, m_Initialized(false)
	, m_IsSnake(false)
	, m_IsDead(true)
	, m_IsPlayerControlled(playerControlled)
	, m_MoveDirection(direction::DownLeft)
	, m_MoveSpeed(60.0f)
	, m_StartPos(0,0)
	, m_CurrentPos(0,0)
	, m_NextPos(0, 0)
	, m_MoveTimer(0.0f)
	, m_MaxMoveTime(2.0f)
	, m_DeathTimer(0.0f)
	, m_MaxDeathTime(7.5f)
	, m_IsFallingDown(false)
	, m_FallTimer(0)
	, m_MaxFallTime(2.0f)
	, m_FallDownDir(0, 1)
	, m_Anim(nullptr)
{
	SetRandomStartPos();
	m_CurrentPos = m_StartPos;
}


void CoilyComponent::UpdateComponent()
{
	//(re)spawning
	if(m_IsDead)
	{
		m_DeathTimer += TimeManager::GetInstance().GetDeltaTime();
		//Set target at intervals
		if (m_DeathTimer >= m_MaxDeathTime)
		{
			m_IsDead = false;
			m_CurrentPos = m_StartPos;
			TransformToEgg();
		}
		else
			return;
	}

	if (m_IsFallingDown)
	{
		m_FallTimer += TimeManager::GetInstance().GetDeltaTime();
		if (m_FallTimer >= m_MaxFallTime)
		{
			m_FallTimer = 0;
			m_IsFallingDown = false;
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
	
	//Set target at intervals
	m_MoveTimer += TimeManager::GetInstance().GetDeltaTime();
	if(m_MoveTimer >= m_MaxMoveTime)
	{
		m_MoveTimer = 0;
		if (m_IsSnake && !m_IsPlayerControlled)
		{
			//move towards qbert
			MoveTowardsPlayer();
		}
		else if(!m_IsSnake)
		{
			//move down
			m_MoveDirection = RandomDirectionDown();
			MoveDown();
		}
	}
	
	//Actually move
	if(m_Move)
	{	
		auto dir = m_NextPos - m_CurrentPos;
		dir = glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);
		if (glm::distance(m_CurrentPos, m_NextPos) <= 1.0f)
		{
			m_Move = false;
			m_CurrentPos = m_NextPos;
		}

		m_pParentObj->GetTransform().SetPosition(m_CurrentPos.x, m_CurrentPos.y, 1.0f);
		
		if (m_Anim)
			m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
	}

	//older player collision
	//auto otherPlayer = LevelManager::GetInstance().GetPlayerOnHexByPos(m_CurrentPos);
	//if (!m_IsDead && otherPlayer && LevelManager::GetInstance().GetIsHexOccupiedByPos(m_CurrentPos))
	//{
	//	otherPlayer->CollisionWithPurpleEnemy();
	//	Die();
	//}
}

void CoilyComponent::PostAddedToGameObject()
{
}

void CoilyComponent::Notify(Event event)
{
	if (event == Event::LevelFinished || event == Event::ActorDied || event == Event::ActorHitPurple)
	{
		Die();
	}
}

void CoilyComponent::move(direction dir)
{
	if (m_Move || !m_IsPlayerControlled || !m_IsSnake || m_IsFallingDown)
		return;

	auto& levelmanager = LevelManager::GetInstance();
	const float fallDownOffset{ 0.3f };
	m_Move = true;
	m_MoveDirection = dir;

	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);

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

	if (levelmanager.GetIsHexValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetHexPosByCoord(currentHexCoord);
	}
	else
	{
		m_IsFallingDown = true;
	}
}

void CoilyComponent::AddTarget(const CharacterComponent* target)
{
	m_Targets.push_back(target);
}

void CoilyComponent::SetAnim(MultiAnimationComponent* anim)
{
	m_Anim = anim;
	if (m_Anim)
	{
		m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
		m_Anim->SetAnimState(AnimState::Invisible);
	}
}

void CoilyComponent::TransformToSnake()
{
	//Transform
	m_IsSnake = true;
	if (m_Anim)
		m_Anim->SetAnimState(AnimState::FacingForward);
}

void CoilyComponent::TransformToEgg()
{
	m_IsSnake = false;
	if (m_Anim)
		m_Anim->SetAnimState(AnimState::None);
}

void CoilyComponent::Die()
{
	SetRandomStartPos();
	m_IsSnake = false;
	m_CurrentPos = m_StartPos;
	m_NextPos = m_CurrentPos;
	m_IsDead = true;
	m_DeathTimer = 0;
	m_IsFallingDown = false;
	m_FallTimer = 0;
	if (m_Anim)
	{
		m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
		m_Anim->SetAnimState(AnimState::Invisible);
	}
	m_Move = false;
}

direction CoilyComponent::RandomDirectionDown()
{
	int random = rand() % 2 + 2;
	const auto dir = (direction)random;
	return dir;
}

void CoilyComponent::SetRandomStartPos()
{
	const float random = float(rand() % 2 - 1);
	auto& levelmanager = LevelManager::GetInstance();
	const glm::vec2 startCoord{ 1,random };
	if (levelmanager.GetIsHexValidByCoord(startCoord))
	{
		m_StartPos = levelmanager.GetHexPosByCoord(startCoord);
	}
}

void CoilyComponent::MoveDown()
{
	auto& levelmanager = LevelManager::GetInstance();
	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);
	
	if (m_MoveDirection == direction::DownLeft)
	{
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
	}
	else if (m_MoveDirection == direction::DownRight)
	{
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
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
			TransformToSnake();
		else
			Logger::GetInstance().Log(LogType::Error, "Coily went wrong somewhere");
	}
}

void CoilyComponent::MoveTowardsPlayer()
{
	auto& levelmanager = LevelManager::GetInstance();
	const auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);
	
	//Get The pos of the currently closest target player
	glm::vec2 closestPos{};
	glm::vec2 closestDiscPos{};
	bool onDisc{ false };
	float distance{ INFINITY };
	const CharacterComponent* tempChar{nullptr};
	for(const auto& target : m_Targets)
	{
		const auto currentDistance = glm::distance(m_CurrentPos, target->GetCurrentCharacterPos());
		if (currentDistance <= distance)
		{
			distance = currentDistance;
			closestPos = target->GetCurrentCharacterPos();
			closestDiscPos = target->GetDiscPos();
			onDisc = target->GetOnDisc();
			tempChar = target;
		}
	}

	//if player on disc and in range then jump off
	auto neighbors = levelmanager.GetNeighboringAccesibleHexes(currentHexCoord);
	glm::vec2 closestPosHex{};
	distance = INFINITY;
	if (onDisc)
	{
		//Get the current neighboring hex closest to the target disc
		const float distanceDisc{ glm::distance(closestDiscPos,m_CurrentPos) };
		for (const auto& hex : neighbors)
		{
			const float currentDistance = glm::distance(closestDiscPos, hex.GetPos());
			if (currentDistance <= distance)
			{
				distance = currentDistance;
				closestPosHex = hex.GetPos();
			}
		}
		const float distanceHex = glm::distance(closestPosHex, m_CurrentPos);

		//jump off when coily would jump on disc
		if (distanceHex >= distanceDisc)
		{
			Logger::GetInstance().Log(LogType::Info, "coily fell of when trying to jump on the disc");
			Die();
			if (tempChar)
			{
				const auto subject = tempChar->GetParentObject()->GetComponent<SubjectComponent>();
				if (subject) subject->Notify(Event::DefeatedCoily);
			}		
			return;
		}
	}
	
	//Get the current neighboring hex closest to the target hex
	for (const auto& hex : neighbors)
	{
		const float currentDistance = glm::distance(closestPos, hex.GetPos());
		if (currentDistance <= distance)
		{
			distance = currentDistance;
			closestPosHex = hex.GetPos();
		}
	}
	auto nextHex = levelmanager.GetHexCoordByClosestPos(closestPosHex);
	
	//Set the closest neighboring hex as next pos
	if (levelmanager.GetIsHexValidByCoord(nextHex))
	{
		m_NextPos = levelmanager.GetHexPosByCoord(nextHex);
		m_Move = true;
	}

	//set animstate depending on direction
	const auto direction = m_NextPos - m_CurrentPos;
	if (direction.x >= 0 && direction.y >= 0)
		if (m_Anim) m_Anim->SetState(AnimState::FacingForward, true);
	if (direction.x <= 0 && direction.y >= 0)
		if (m_Anim) m_Anim->SetState(AnimState::FacingForward, false);
	if (direction.x >= 0 && direction.y <= 0)
		if (m_Anim) m_Anim->SetState(AnimState::FacingAway, true);
	if (direction.x <= 0 && direction.y <= 0)
		if (m_Anim) m_Anim->SetState(AnimState::FacingAway, false);

}
