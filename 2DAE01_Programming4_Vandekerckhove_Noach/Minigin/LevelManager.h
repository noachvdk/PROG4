#pragma once
#include "HexagonalGrid.h"
#include "LevelSettings.h"
#include "Singleton.h"

namespace dae
{
	class LevelManager final : public Singleton<LevelManager>
	{
	public:
		void LoadLevelWithRawHexCoords(const std::string& layout, const std::string& settings) const;
		void LoadLevelWithShape(const std::string& layout, const std::string& settings) const;

		int GetCurrentLevelID()const { return m_CurrentLevelID; }
		LevelSetting GetCurrentSettings() const;
		const std::shared_ptr<Texture2D> GetHexTexture(int id) const;
		int GetAmountOfLevels() const { return m_LevelSettings.GetAmountOfLevels(); }
		int GetAmountOfSteps() const { return m_HexGridManager.GetAmountOfSteps(); }
		glm::vec2 GetGridOrigin()const { return m_HexGridManager.GetBaseGridPos(); }
		glm::vec2 GetHexCoordByClosestPos(const glm::vec2& pos)const { return m_HexGridManager.GetCoordByClosestPos(pos); }
		glm::vec2 GetHexPosByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetHexPosByCoord(coord); }
		bool GetIsHexValidByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetIsHexValidByCoord(coord); }
		bool GetAreAllHexesFlipped()const { return m_HexGridManager.GetAreAllHexesFlipped(); }
		bool GetIsHexFlippedByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetIsHexAlreadyFlippedByCoord(coord); }

		void SetCurrentLevelID(int newID);
		void NextLevel();
		void ChangeHexColorByPos(const glm::vec2& pos);
		
		void Render();
	private:
		friend class Singleton<LevelManager>;
		LevelManager() = default;
		int m_CurrentLevelID;
		static LevelSettings m_LevelSettings;
		static HexagonalGridManager m_HexGridManager;
	};
}

