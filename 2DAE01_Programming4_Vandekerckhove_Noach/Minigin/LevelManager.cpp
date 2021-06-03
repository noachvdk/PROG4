#include "MiniginPCH.h"
#include "LevelManager.h"

#include "Logger.h"

using namespace dae;

LevelSettings LevelManager::m_LevelSettings;
HexagonalGridManager LevelManager::m_HexGridManager;

LevelManager::LevelManager()
	:m_CurrentLevelID(0)
	,m_Disc01(nullptr)
	,m_Disc02(nullptr)
{
	//m_Disc01 = std::make_shared<Disc>(m_HexGridManager.GetBaseGridPos(), m_HexGridManager.GetRadius(), false);
	//m_Disc02 = std::make_shared<Disc>(m_HexGridManager.GetBaseGridPos(), m_HexGridManager.GetRadius(), true);
}

void LevelManager::LoadLevelWithRawHexCoords(const std::string& layout, const std::string& settings)
{
	m_LevelSettings.LoadLevelSettings(settings);
	m_HexGridManager.Init(m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded, m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack);
	m_HexGridManager.loadFromFileRawHex(layout);

	m_Disc01 = std::make_shared<Disc>(m_HexGridManager.GetBaseGridPos(), m_HexGridManager.GetRadius(), false);
	m_Disc02 = std::make_shared<Disc>(m_HexGridManager.GetBaseGridPos(), m_HexGridManager.GetRadius(), true);
}

void LevelManager::LoadLevelWithShape(const std::string& layout, const std::string& settings) 
{
	m_LevelSettings.LoadLevelSettings(settings);
	m_HexGridManager.Init(m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_JumpsNeeded, m_LevelSettings.GetCurrentSettings(m_CurrentLevelID).m_CanChangeBack);
	m_HexGridManager.loadFromFileShape(layout);

	m_Disc01 = std::make_shared<Disc>(m_HexGridManager.GetBaseGridPos(), m_HexGridManager.GetRadius(), false);
	m_Disc02 = std::make_shared<Disc>(m_HexGridManager.GetBaseGridPos(), m_HexGridManager.GetRadius(), true);
}

LevelSetting LevelManager::GetCurrentSettings() const
{
	return m_LevelSettings.GetCurrentSettings(m_CurrentLevelID);
}

const std::shared_ptr<Texture2D> LevelManager::GetHexTexture(int id) const
{
	return m_HexGridManager.GetHexTexture(id);
}

bool LevelManager::GetIsDiscValidByCoord(const glm::vec2& coord) const
{
	if (m_Disc01 && m_Disc01->GetCoord() == coord)
		return true;

	if (m_Disc02 && m_Disc02->GetCoord() == coord)
		return true;
	
	return false;
}

glm::vec2 LevelManager::GetDiscPosByCoord(const glm::vec2& coord) const
{
	if (m_Disc01 && m_Disc01->GetCoord() == coord)
		return m_Disc01->GetPos();

	if (m_Disc02 && m_Disc02->GetCoord() == coord)
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

float LevelManager::GetDiscMoveSpeed() const
{
	if (m_Disc01)
		return m_Disc01->GetDiscSpeed();
	if (m_Disc02)
		return m_Disc02->GetDiscSpeed();
	return 0.0f;
}

bool LevelManager::GetIsDiscSteppedOnByCoord(const glm::vec2& coord) const
{
	if (m_Disc01 && m_Disc01->GetCoord() == coord)
		return m_Disc01->GetIsSteppedOn();

	if (m_Disc02 && m_Disc02->GetCoord() == coord)
		return m_Disc02->GetIsSteppedOn();

	return false;
}

int LevelManager::GetAmountOfDiscsRemaining() const
{
	int amount{};
	if (m_Disc01 && !m_Disc01->GetIsUsed())
		amount++;

	if (m_Disc02 && !m_Disc02->GetIsUsed())
		amount++;
	return amount;
}

void LevelManager::SetDiscSteppedOn(const glm::vec2& coord) const
{
	if (m_Disc01 && m_Disc01->GetCoord() == coord)
		m_Disc01->SetSteppedOn();

	if (m_Disc02 && m_Disc02->GetCoord() == coord)
		m_Disc02->SetSteppedOn();
}

void LevelManager::SetCurrentLevelID(int newID)
{
	m_CurrentLevelID = newID;

	m_CurrentLevelID = std::max(1, m_CurrentLevelID);
	m_CurrentLevelID = std::min(m_CurrentLevelID, m_LevelSettings.GetAmountOfLevels());
	
	const auto setting = m_LevelSettings.GetCurrentSettings(m_CurrentLevelID);
	m_HexGridManager.SetNewLevel(setting.m_JumpsNeeded, setting.m_CanChangeBack);

	if (setting.m_CanChangeBack == 0)
	{
		Logger::GetInstance().Log(LogType::Info, "Current Level with ID " + std::to_string(setting.m_LevelID) + " needs " + std::to_string(setting.m_JumpsNeeded) + " jumps to change to last color and can't be changed back");
	}
	else
	{
		Logger::GetInstance().Log(LogType::Info, "Current Level with ID " + std::to_string(setting.m_LevelID) + " needs " + std::to_string(setting.m_JumpsNeeded) + " jumps to change to last color and can be changed back");
	}

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

	const auto setting = m_LevelSettings.GetCurrentSettings(m_CurrentLevelID);
	if (setting.m_CanChangeBack == 0)
	{
		Logger::GetInstance().Log(LogType::Info, "Current Level with ID " + std::to_string(setting.m_LevelID) + " needs " + std::to_string(setting.m_JumpsNeeded) + " jumps to change to last color and can't be changed back");
	}
	else
	{
		Logger::GetInstance().Log(LogType::Info, "Current Level with ID " + std::to_string(setting.m_LevelID) + " needs " + std::to_string(setting.m_JumpsNeeded) + " jumps to change to last color and can be changed back");
	}

	if (m_Disc01)
		m_Disc01->SetRandomCoord(GetAmountOfSteps());

	if (m_Disc02)
		m_Disc02->SetRandomCoord(GetAmountOfSteps());
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
