#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "LevelManager.h"
#include "SubjectComponent.h"
#include "FontComponent.h"
#include "GameSettings.h"
#include "SceneManager.h"
#include "TimeManager.h"

using namespace dae;

LevelComponent::LevelComponent()
	: m_LevelID(1)
	, m_Flash(false)
	, m_Switch(false)
	, m_Victory(true)
	, m_FlashTimer(0)
	, m_MaxFlashTime(1.5f)
	, m_FlashSwitchTimer(0)
	, m_MaxFlashSwitchTime(0.25f)
	, m_pFontComponent(nullptr)
	, m_LevelCompleted(false)
	, m_pCompletedFontComponent(nullptr)
{
	m_LevelID = std::max(1, m_LevelID);
	m_LevelID = std::min(m_LevelID, LevelManager::GetInstance().GetAmountOfLevels());
	m_pFontComponent = new FontComponent{ "Cooper.ttf",16," " ,true};
	m_pFontComponent->SetText("Level : " + std::to_string(m_LevelID));
	m_pCompletedFontComponent = new FontComponent{ "Cooper.ttf",32," ",true };
	m_pCompletedFontComponent->SetText("Level Completed");
	m_pCompletedFontComponent->SetVisible(false);
}

void LevelComponent::UpdateComponent()
{
	auto& levelmanager = LevelManager::GetInstance();

	levelmanager.Update();

	if(m_Flash)
	{
		m_FlashTimer += TimeManager::GetInstance().GetDeltaTime();
		m_FlashSwitchTimer += TimeManager::GetInstance().GetDeltaTime();

		if (m_FlashSwitchTimer >= m_MaxFlashSwitchTime)
		{
			if (!m_Switch)
				levelmanager.SetAllHexColors(0);
			else
				levelmanager.SetAllHexColors(1);
			m_FlashSwitchTimer = 0.0f;
			m_Switch = !m_Switch;
		}

		if (m_FlashTimer >= m_MaxFlashTime)
		{
			m_FlashTimer = 0;
			m_FlashSwitchTimer = 0.0f;
			m_Flash = false;
			m_Switch = false;
			if(m_Victory)
				levelmanager.SetCurrentLevelID(m_LevelID);
			else
			{
				GameSettings::GetInstance().SetGameMode(GameMode::NotChosen);
				SceneManager::GetInstance().SetCurrentSceneName("MainMenu");
			}
			m_pCompletedFontComponent->SetVisible(false);
		}
	}
	else if (levelmanager.GetAreAllHexesFlipped())
	{
		NextLevel();
	}
}

void LevelComponent::RenderComponent()
{
	LevelManager::GetInstance().Render();
}

void LevelComponent::PostAddedToGameObject()
{
	GetParentObject()->AddComponent(m_pFontComponent);
	m_pFontComponent->AddOffset(m_Offset.x, m_Offset.y);
	GetParentObject()->AddComponent(m_pCompletedFontComponent);
	m_pCompletedFontComponent->AddOffset(m_CompletedOffset.x, m_CompletedOffset.y);
}

void LevelComponent::Notify(Event event)
{
	if(event == Event::ActorDied)
	{
		const auto subject = m_pParentObj->GetComponent<SubjectComponent>();
		if (subject) subject->Notify(Event::Reset);
		m_pCompletedFontComponent->SetVisible(true);
		m_pCompletedFontComponent->SetText("Level Failed");
		m_Flash = true;
		m_Victory = false;
	}
}

void LevelComponent::NextLevel()
{
	m_LevelID++;
	m_Flash = true;
	m_Victory = true;
	m_pCompletedFontComponent->SetVisible(true);
	m_pCompletedFontComponent->SetText("Level Completed");
	m_pFontComponent->SetText("Level : " + std::to_string(m_LevelID));
	const auto subject = GetParentObject()->GetComponent<SubjectComponent>();
	if (subject)
		subject->Notify(Event::LevelFinished);
}
