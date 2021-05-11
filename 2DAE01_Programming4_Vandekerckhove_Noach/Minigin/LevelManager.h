#pragma once
#include "Disc.h"
#include "HexagonalGrid.h"
#include "LevelSettings.h"
#include "Singleton.h"

namespace dae
{
	class LevelManager final : public Singleton<LevelManager>
	{
	public:
		//Parsing
		void LoadLevelWithRawHexCoords(const std::string& layout, const std::string& settings) const;
		void LoadLevelWithShape(const std::string& layout, const std::string& settings) const;

		//General
		int GetCurrentLevelID()const { return m_CurrentLevelID; }
		LevelSetting GetCurrentSettings() const;
		const std::shared_ptr<Texture2D> GetHexTexture(int id) const;
		int GetAmountOfLevels() const { return m_LevelSettings.GetAmountOfLevels(); }
		int GetAmountOfSteps() const { return m_HexGridManager.GetAmountOfSteps(); }
		glm::vec2 GetGridOrigin()const { return m_HexGridManager.GetBaseGridPos(); }

		//Hex
		glm::vec2 GetHexCoordByClosestPos(const glm::vec2& pos)const { return m_HexGridManager.GetCoordByClosestPos(pos); }
		glm::vec2 GetHexPosByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetHexPosByCoord(coord); }
		bool GetIsHexValidByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetIsHexValidByCoord(coord); }
		bool GetAreAllHexesFlipped()const { return m_HexGridManager.GetAreAllHexesFlipped(); }
		bool GetIsHexFlippedByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetIsHexAlreadyFlippedByCoord(coord); }

		//Disc
		void SetDisc01(const glm::vec2& pos);
		void SetDisc02(const glm::vec2& pos);
		bool GetIsDiscValidByCoord(const glm::vec2& coord)const;
		glm::vec2 GetDiscPosByCoord(const glm::vec2& coord)const;
		glm::vec2 GetDiscTopPos()const;
		void SetHexSteppedOn(const glm::vec2& coord)const;

		//
		void SetCurrentLevelID(int newID);
		void NextLevel();
		void ChangeHexColorByPos(const glm::vec2& pos);
		
		void Render()const;
		void Update()const;
	private:
		friend class Singleton<LevelManager>;
		LevelManager() = default;
		int m_CurrentLevelID;
		static LevelSettings m_LevelSettings;
		static HexagonalGridManager m_HexGridManager;
		std::shared_ptr<Disc> m_Disc01;
		std::shared_ptr<Disc> m_Disc02;
	};
}

