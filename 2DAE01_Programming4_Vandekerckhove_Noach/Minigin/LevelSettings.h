#pragma once

struct LevelSetting
{
	int m_LevelID;
	int m_JumpsNeeded;
	bool m_CanChangeBack;
};

namespace dae
{
	class LevelSettings
	{
	public:
		LevelSettings() = default;
		void LoadLevelSettings(const std::string& path);
		LevelSetting GetCurrentSettings(int ID);
		int GetAmountOfLevels() const { return int(m_Levels.size()); }
	private:
		std::vector<LevelSetting> m_Levels;
	};
}


