#include "MiniginPCH.h"
#include "UggOrWrongwayComponent.h"
#include "TimeManager.h"
#include "MultiAnimationComponent.h"
#include "LevelManager.h"
#include "Logger.h"

using namespace dae;

UggOrWrongwayComponent::UggOrWrongwayComponent(Type type)
	: m_Move(false)
	, m_IsDead(true)
	, m_DeathTimer(0.0f)
	, m_MaxDeathTime(5.0f)
	, m_CurrentType(type)
	, m_MoveDirection(direction::UpLeft)
	, m_MoveSpeed(1.0f)
	, m_MoveTimer(0.0f)
	, m_MaxMoveTime(2.5f)
	, m_StartPos(0, 0)
	, m_CurrentPos(0, 0)
	, m_NextPos(0, 0)
	, m_Offset(0,0)
	, m_Anim(nullptr)
{
	//start on the bottom side of the pyramid
	const int steps = LevelManager::GetInstance().GetAmountOfSteps();
	if (m_CurrentType == Type::Ugg)
	{
		const int yOffset = (steps + 1) / 2;
		m_StartPos = LevelManager::GetInstance().GetHexPosByCoord(glm::vec2(steps, -yOffset));
	}
	else
	{
		int yOffset = ((steps + 1) / 2) - 1;
		if (steps % 2 == 0)
			yOffset++;
		m_StartPos = LevelManager::GetInstance().GetHexPosByCoord(glm::vec2(steps, yOffset));
	}
	m_CurrentPos = m_StartPos;
}

void UggOrWrongwayComponent::UpdateComponent()
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
		m_MoveDirection = RandomDirection();
		Move();
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

	//old if hit player
	//auto pos = GetHexCollisionPos(); //hex pos taking in account the Escheresque manner they move
	//if(pos != glm::vec2(0,0))
	//{
	//	auto otherPlayer = LevelManager::GetInstance().GetPlayerOnHexByPos(pos);
	//	if (!m_IsDead && otherPlayer && LevelManager::GetInstance().GetIsHexOccupiedByPos(pos))
	//	{
	//		Die();
	//		otherPlayer->CollisionWithPurpleEnemy();
	//	}
	//}
}

void UggOrWrongwayComponent::Notify(Event event)
{
	if (event == Event::LevelFinished || event == Event::ActorDied || event == Event::ActorHitPurple)
	{
		Die();
	}
}

void UggOrWrongwayComponent::SetAnimComponent(MultiAnimationComponent* anim)
{
	m_Anim = anim;
	if (m_Anim)
	{
		m_Anim->SetPos(m_CurrentPos.x, m_CurrentPos.y);
		const auto temp = m_Anim->GetAnimComponent(AnimState::None);
		float x{}, y{};
		//set offset to they appear on the sides
		if(temp && m_CurrentType == Type::Ugg)
		{
			x = temp->GetTextureFrameWidth() * -1.0f;
			y = temp->GetTextureFrameHeight() * 1.5f;
		}
		else if(temp && m_CurrentType == Type::WrongWay)
		{
			x = temp->GetTextureFrameWidth() * 1.0f;
			y = temp->GetTextureFrameHeight() * 1.5f;
		}
		m_Anim->SetOffset(x, y);
	}
}

void UggOrWrongwayComponent::Die()
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

direction UggOrWrongwayComponent::RandomDirection()
{
	int random{ rand() % 2 + 4 };
	const auto dir = (direction)random;
	return dir;
}

void UggOrWrongwayComponent::Move()
{
	auto& levelmanager = LevelManager::GetInstance();
	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);
	
	if (m_MoveDirection == direction::Horizontal)
	{
		if(m_CurrentType == Type::Ugg)
			currentHexCoord.y += 1;
		else
			currentHexCoord.y -= 1;

	}
	else if (m_MoveDirection == direction::Vertical)
	{
		currentHexCoord.x -= 1;
		if (m_CurrentType == Type::Ugg)
		{
			if (int(currentHexCoord.x) % 2 == 0)
				currentHexCoord.y += 1;
		}
		else
		{
			if (int(currentHexCoord.x) % 2 != 0)
				currentHexCoord.y -= 1;
		}
		
	}

	if (levelmanager.GetIsHexValidByCoord(currentHexCoord))
	{
		m_NextPos = levelmanager.GetHexPosByCoord(currentHexCoord);
		m_Move = true;
	}
	else
	{
		Die();
	}
}

glm::vec2 UggOrWrongwayComponent::GetHexCollisionPos() const
{
	auto& levelmanager = LevelManager::GetInstance();
	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);
	
	if(m_CurrentType == Type::Ugg)
	{
		currentHexCoord.x += 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
	}
	else
	{
		currentHexCoord += 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
	}
	
	if(levelmanager.GetIsHexValidByCoord(currentHexCoord))
		return levelmanager.GetHexPosByCoord(currentHexCoord);
	
	return glm::vec2(0,0);
}
