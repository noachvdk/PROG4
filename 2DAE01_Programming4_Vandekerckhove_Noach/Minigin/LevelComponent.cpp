#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "LevelManager.h"
#include "SubjectComponent.h"
#include "FontComponent.h"

using namespace dae;

LevelComponent::LevelComponent()
	: m_LevelID(1)
	, m_pFontComponent(nullptr)
{
	m_LevelID = std::max(1, m_LevelID);
	m_LevelID = std::min(m_LevelID, LevelManager::GetInstance().GetAmountOfLevels());
	m_pFontComponent = new FontComponent{ "Lingua.otf",16," " };
	m_pFontComponent->SetText("Level : " + std::to_string(m_LevelID));
}

void LevelComponent::UpdateComponent()
{
	auto& levelmanager = LevelManager::GetInstance();

	levelmanager.Update();

	//broadcast event here
	if (levelmanager.GetAreAllHexesFlipped())
	{
		/*std::cout << "finished\n";*/
		NextLevel();
		m_pFontComponent->SetText("Level : " + std::to_string(m_LevelID));
		const auto subject = GetParentObject()->GetComponent<SubjectComponent>();
		if (subject)
			subject->Notify(Event::LevelFinished);
	}
		
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


void LevelComponent::NextLevel()
{
	m_LevelID++;
	m_LevelID = std::min(m_LevelID, LevelManager::GetInstance().GetAmountOfLevels());
	LevelManager::GetInstance().SetCurrentLevelID(m_LevelID);
}
