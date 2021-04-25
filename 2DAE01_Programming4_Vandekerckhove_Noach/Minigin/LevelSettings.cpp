#include "MiniginPCH.h"
#include "LevelSettings.h"
#include <iostream>
#include <fstream>
#include "ResourceManager.h"

void dae::LevelSettings::LoadLevelSettings(const std::string& path)
{
	std::ifstream file{ dae::ResourceManager::GetInstance().GetFullPath() + path };
	if (!file)
		std::cout << "file not opened " << dae::ResourceManager::GetInstance().GetFullPath() + path << "\n";


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
		std::cout << "Level with ID " << lvl.m_LevelID << " needs " << lvl.m_JumpsNeeded <<" jumps to change to last color and " <<lvl.m_CanChangeBack <<"\n";
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
