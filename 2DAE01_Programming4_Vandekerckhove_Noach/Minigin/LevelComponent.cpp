#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "LevelManager.h"
#include "SubjectComponent.h"
#include "FontComponent.h"

using namespace dae;

LevelComponent::LevelComponent(const int levelID)
	:m_LevelID(levelID)
	, m_NeedsUpdate(true)
{
	m_LevelID = std::max(1, m_LevelID);
	m_LevelID = std::min(m_LevelID, LevelManager::GetInstance().GetAmountOfLevels());
	m_pFontComponent = new FontComponent{ "Lingua.otf",16," " };
}

void LevelComponent::UpdateComponent()
{
	auto& levelmanager = LevelManager::GetInstance();

	levelmanager.Update();
	
	if (levelmanager.GetCurrentLevelID() != m_LevelID)
	{
		levelmanager.SetCurrentLevelID(m_LevelID);
		m_NeedsUpdate = true;
	}


	//broadcast event here
	if (levelmanager.GetAreAllHexesFlipped())
	{
		/*std::cout << "finished\n";*/
		m_LevelID++;
		if (m_LevelID > levelmanager.GetAmountOfLevels())
			m_LevelID = 1;
		const auto subject = GetParentObject()->GetComponent<SubjectComponent>();
		if (subject)
			subject->Notify(Event::LevelFinished);
	}

	if(m_NeedsUpdate)
	m_pFontComponent->SetText("Level : " + std::to_string(m_LevelID));
		
}

void LevelComponent::RenderComponent()
{
	LevelManager::GetInstance().Render();
}

void LevelComponent::PostAddedToGameObject()
{
	GetParentObject()->AddComponent(m_pFontComponent);
	m_pFontComponent->AddOffset(GetParentObject()->GetTransform().GetPosition().x + m_Offset.x, -GetParentObject()->GetTransform().GetPosition().y + m_Offset.y);
}

void LevelComponent::Notify(Event)
{
	//if (event == Event::LevelFinished)
	//	NextLevel();
}

void LevelComponent::NextLevel()
{
	m_LevelID++;
	m_LevelID = std::min(m_LevelID, LevelManager::GetInstance().GetAmountOfLevels());
}
