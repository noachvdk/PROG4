#include "MiniginPCH.h"
#include "LevelManager.h"

using namespace dae;

LevelSettings LevelManager::m_LevelSettings;
HexagonalGridManager LevelManager::m_HexGridManager;

void LevelManager::LoadLevelWithRawHexCoords(const std::string& layout, const std::string& settings) const
{
	m_LevelSettings.LoadLevelSettings(settings);
	m_HexGridManager.Init(m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded, m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack);
	m_HexGridManager.loadFromFileRawHex(layout);
}

void LevelManager::LoadLevelWithShape(const std::string& layout, const std::string& settings) const
{
	m_LevelSettings.LoadLevelSettings(settings);
	m_HexGridManager.Init(m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded, m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack);
	m_HexGridManager.loadFromFileShape(layout);
}

LevelSetting LevelManager::GetCurrentSettings() const
{
	return m_LevelSettings.GetCurrentSettings(m_CurrentLevelID);
}

const std::shared_ptr<Texture2D> LevelManager::GetHexTexture(int id) const
{
	return m_HexGridManager.GetHexTexture(id);
}

void LevelManager::SetCurrentLevelID(int newID)
{
	m_CurrentLevelID = newID;

	m_CurrentLevelID = std::max(1, m_CurrentLevelID);
	m_CurrentLevelID = std::min(m_CurrentLevelID, m_LevelSettings.GetAmountOfLevels());
	
	m_HexGridManager.SetNewLevel(m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded, m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack);

	std::cout << "current Level with ID " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_LevelID << " needs " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded << " jumps to change to last color and " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack << "\n";
}

void LevelManager::NextLevel()
{
	m_CurrentLevelID++;
	
	m_CurrentLevelID = std::max(1, m_CurrentLevelID);
	m_CurrentLevelID = std::min(m_CurrentLevelID, m_LevelSettings.GetAmountOfLevels());
	
	m_HexGridManager.SetNewLevel(m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded, m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack);

	std::cout << "current Level with ID " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_LevelID << " needs " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded << " jumps to change to last color and " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack << "\n";
}

void LevelManager::ChangeHexColorByPos(const glm::vec2& pos)
{
	m_HexGridManager.changeClosestHexByPos(pos);
}

void LevelManager::Render()
{
	m_HexGridManager.Render();
}
