#include "MiniginPCH.h"
#include "LevelSettings.h"
#include <fstream>

#include "Logger.h"
#include "ResourceManager.h"

void dae::LevelSettings::LoadLevelSettings(const std::string& path)
{
	//TIME();
	std::ifstream file{ dae::ResourceManager::GetInstance().GetFullPath() + path };
	if (!file)
		Logger::GetInstance().Log(LogType::Error, "file not opened " + ResourceManager::GetInstance().GetFullPath() + path);


	std::string str;
	while (std::getline(file, str))
	{
		LevelSetting lvl;
		std::stringstream test(str);
		std::string level;
		std::string id;
		std::string jumps;
		std::string canChangeBack;
		std::getline(test,level,' ');
		std::getline(test, id, ' ');
		std::getline(test, jumps, ' ');
		std::getline(test, canChangeBack, ' ');

		lvl.m_LevelID = std::stoi(id);
		lvl.m_JumpsNeeded = std::stoi(jumps);
		lvl.m_CanChangeBack = (canChangeBack != "false");
		if(lvl.m_CanChangeBack ==0)
		{
			Logger::GetInstance().Log(LogType::Info, "Loaded Level with ID " + std::to_string(lvl.m_LevelID) + " needs " + std::to_string(lvl.m_JumpsNeeded) + " jumps to change to last color and can't be changed back");
		}
		else
		{
			Logger::GetInstance().Log(LogType::Info, "Loaded Level with ID " + std::to_string(lvl.m_LevelID) + " needs " + std::to_string(lvl.m_JumpsNeeded) + " jumps to change to last color and can be changed back");
		}
		m_Levels.push_back(lvl);
	}

}

LevelSetting dae::LevelSettings::GetCurrentSettings(int ID)
{
	for (auto level : m_Levels)
	{
		if (level.m_LevelID == ID)
			return level;
	}
	return LevelSetting();
}
