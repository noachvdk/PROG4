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

void LevelManager::SetDisc01(const glm::vec2& pos)
{
	m_Disc01 = std::make_shared<Disc>(pos, m_HexGridManager.GetBaseGridPos(), 30);
}

void LevelManager::SetDisc02(const glm::vec2& pos)
{
	m_Disc02 = std::make_shared<Disc>(pos, m_HexGridManager.GetBaseGridPos(), 30);
}

bool LevelManager::GetIsDiscValidByCoord(const glm::vec2& coord) const
{
	if (m_Disc01->GetCoord() == coord)
		return true;

	if (m_Disc02->GetCoord() == coord)
		return true;
	
	return false;
}

glm::vec2 LevelManager::GetDiscPosByCoord(const glm::vec2& coord) const
{
	if (m_Disc01->GetCoord() == coord)
		return m_Disc01->GetPos();

	if (m_Disc02->GetCoord() == coord)
		return m_Disc02->GetPos();
	
	return glm::vec2();
}

glm::vec2 LevelManager::GetDiscTopPos() const
{
	if (m_Disc01)
		return m_Disc01->GetOrigin();
	if (m_Disc02)
		return m_Disc02->GetOrigin();
	return glm::vec2();
}

void LevelManager::SetHexSteppedOn(const glm::vec2& coord) const
{
	if (m_Disc01->GetCoord() == coord)
		m_Disc01->SetSteppedOn();

	if (m_Disc02->GetCoord() == coord)
		m_Disc02->SetSteppedOn();
}

void LevelManager::SetCurrentLevelID(int newID)
{
	m_CurrentLevelID = newID;

	m_CurrentLevelID = std::max(1, m_CurrentLevelID);
	m_CurrentLevelID = std::min(m_CurrentLevelID, m_LevelSettings.GetAmountOfLevels());
	
	m_HexGridManager.SetNewLevel(m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded, m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack);

	std::cout << "current Level with ID " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_LevelID << " needs " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded << " jumps to change to last color and " << m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack << "\n";

	if (m_Disc01)
		m_Disc01->Reset();
	if (m_Disc02)
		m_Disc02->Reset();
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

void LevelManager::Render()const
{
	m_HexGridManager.Render();
	if(m_Disc01 && !m_Disc01->GetIsUsed())
		m_Disc01->Render();
	if (m_Disc02 && !m_Disc02->GetIsUsed())
		m_Disc02->Render();
}

void LevelManager::Update() const
{
	if (m_Disc01 && !m_Disc01->GetIsUsed())
		m_Disc01->Update();
	if (m_Disc02 && !m_Disc02->GetIsUsed())
		m_Disc02->Update();
}
