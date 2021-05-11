#include "MiniginPCH.h"
#include "CharacterComponent.h"
#include "TextureComponent.h"
#include "LevelManager.h"
#include "LivesComponent.h"
#include "SubjectComponent.h"
#include "TimeManager.h"
#include "GameSettings.h"
#include "PlayerComponent.h"

using namespace dae;

CharacterComponent::CharacterComponent()
	: m_Move(false)
	, m_Initialized(false)
	, m_MoveDirection(direction::DownLeft)
	, m_MoveSpeed(5.0f)
	, m_TextureSize(20)
	, m_StartPos(LevelManager::GetInstance().GetGridOrigin())
	, m_CurrentPos(LevelManager::GetInstance().GetGridOrigin())
	, m_NextPos(0, 0)
	, m_OnDisc(false)
{
}

void CharacterComponent::move(direction dir)
{
	if (m_Move || m_OnDisc)
		return;

	auto& levelmanager = LevelManager::GetInstance();
	m_Move = true;
	m_MoveDirection = dir;

	auto currentHexCoord = levelmanager.GetHexCoordByClosestPos(m_CurrentPos);
	
	//std::cout << currentHexCoord.x << " and " << currentHexCoord.y << "\n";
	//std::cout << m_CurrentPos.x << " and " << m_CurrentPos.y << "\n";

	if (m_MoveDirection == direction::UpLeft)
	{
		currentHexCoord.x -= 1;
		if (int(currentHexCoord.x) % 2 != 0)
			currentHexCoord.y -= 1;
	}
	else if (m_MoveDirection == direction::UpRight)
	{
		currentHexCoord.x -= 1;
		if (int(currentHexCoord.x) % 2 == 0)
			currentHexCoord.y += 1;
	}
	else if (m_MoveDirection == direction::DownLeft)
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
	//std::cout << currentHexCoord.x << " and " << currentHexCoord.y << "\n";
	//std::cout << m_NextPos.x << " and " << m_NextPos.y << "\n";
	//
	if (m_MoveDirection == direction::UpLeft)
		subject->Notify(Event::ActorMoveUpLeft);
	else if (m_MoveDirection == direction::UpRight)
		subject->Notify(Event::ActorMoveUpRight);
	else if (m_MoveDirection == direction::DownLeft)
		subject->Notify(Event::ActorMoveDownLeft);
	else if (m_MoveDirection == direction::DownRight)
		subject->Notify(Event::ActorMoveDownRight);
}

void CharacterComponent::UpdateComponent()
{
	//when the scene loads this will do it once
	if(!m_Initialized)
	{
		if(GameSettings::GetInstance().GetGameMode() == GameMode::Coop)
		{
			auto player = GetParentObject()->GetComponent<PlayerComponent>();
			if (player)
			{
				int id = player->GetPlayerID();
				int steps = LevelManager::GetInstance().GetAmountOfSteps();
				int yOffset{};
				if (id == 0)
				{
					yOffset = (steps + 1) / 2;
					m_StartPos = LevelManager::GetInstance().GetHexPosByCoord(glm::vec2(steps, -yOffset));
				}
				else if (id == 1)
				{
					yOffset = ((steps + 1) / 2)-1;
					if (steps % 2 == 0)
						yOffset++;
					m_StartPos = LevelManager::GetInstance().GetHexPosByCoord(glm::vec2(steps, yOffset));
				}
			}
		}
		m_CurrentPos = m_StartPos;
		m_Initialized = true;
	}

	if (m_IsDead)
		return;
	
	//move pos if needed
	if(m_Move && !m_OnDisc)
	{
		//update currentpos to nextpos using deltatime and a dir vector

		auto dir = m_NextPos - m_CurrentPos;
		glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);

		if (glm::distance(m_CurrentPos, m_NextPos) <= 1.0f)
		{
			m_Move = false;
			m_CurrentPos = m_NextPos;
		}
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
			LevelManager::GetInstance().SetHexSteppedOn(m_CurrentCoord);
		}
	}
	else if(m_OnDisc)
	{
		glm::vec2 topPos = LevelManager::GetInstance().GetDiscTopPos();
		auto dir = topPos - m_CurrentPos;
		glm::normalize(dir);

		m_CurrentPos += (dir * TimeManager::GetInstance().GetDeltaTime() * m_MoveSpeed);

		if (glm::distance(m_CurrentPos, topPos) <= 1.0f)
		{
			m_Move = false;
			m_OnDisc = false;
			m_CurrentPos = m_StartPos;
		}
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
	}
	else if(event == Event::ActorDied)
	{
		m_CurrentPos = m_StartPos;
		m_NextPos = m_CurrentPos;
		m_Move = false;
		//m_IsDead = true;
	}
}
