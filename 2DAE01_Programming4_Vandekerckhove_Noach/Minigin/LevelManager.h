#pragma once
#include "Disc.h"
#include "HexagonalGrid.h"
#include "LevelSettings.h"
#include "Singleton.h"

namespace dae
{
	class CharacterComponent;
	class LevelManager final : public Singleton<LevelManager>
	{
	public:
		//Parsing
		void LoadLevelWithRawHexCoords(const std::string& layout, const std::string& settings) const;
		void LoadLevelWithShape(const std::string& layout, const std::string& settings) const;

		//LevelSettings
		int GetAmountOfLevels() const { return m_LevelSettings.GetAmountOfLevels(); }
		LevelSetting GetCurrentSettings() const;
		
		//General Getters
		int GetCurrentLevelID()const { return m_CurrentLevelID; }
		const std::shared_ptr<Texture2D> GetHexTexture(int id) const;
		int GetAmountOfSteps() const { return m_HexGridManager.GetAmountOfSteps(); }
		glm::vec2 GetGridOrigin()const { return m_HexGridManager.GetBaseGridPos(); }

		//Get Hex
		std::shared_ptr<Hex> getHexByPos(const glm::vec2& pos) const { return m_HexGridManager.getHexByPos(pos); }
		std::shared_ptr<Hex> getHexByCoord(const glm::vec2& coord) const { return m_HexGridManager.getHexByCoord(coord); }
		glm::vec2 GetHexCoordByClosestPos(const glm::vec2& pos)const { return m_HexGridManager.GetCoordByClosestPos(pos); }
		glm::vec2 GetHexPosByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetHexPosByCoord(coord); }
		bool GetIsHexValidByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetIsHexValidByCoord(coord); }
		bool GetAreAllHexesFlipped()const { return m_HexGridManager.GetAreAllHexesFlipped(); }
		bool GetIsHexFlippedByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetIsHexAlreadyFlippedByCoord(coord); }
		bool GetIsHexOccupiedByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetIsHexOccupiedByCoord(coord); }
		bool GetIsHexOccupiedByPos(const glm::vec2& pos)const { return m_HexGridManager.GetIsHexOccupiedByPos(pos); }
		CharacterComponent* GetPlayerOnHexByCoord(const glm::vec2& coord)const { return m_HexGridManager.GetCharacterOnHexByCoord(coord); }
		CharacterComponent* GetPlayerOnHexByPos(const glm::vec2& pos)const { return m_HexGridManager.GetCharacterOnHexByPos(pos); }
		std::vector<Hex> GetNeighboringHexes(const glm::vec2& coord) const { return m_HexGridManager.GetNeighboringHexesByCoord(coord); }
		std::vector<Hex> GetNeighboringAccesibleHexes(const glm::vec2& coord) const { return m_HexGridManager.GetNeighboringAccesibleHexesByCoord(coord); }
		//Set Hex
		static void SetPlayerStandingOnHex(const glm::vec2& pos, bool value) { m_HexGridManager.SetPlayerStandingOnHexByPos(pos, value); }
		static void SetPlayerStandingOnHex(const glm::vec2& pos, CharacterComponent* value) { m_HexGridManager.SetPlayerStandingOnHexByPos(pos, value); }
		void ResetHexByPos(const glm::vec2& pos)const { return m_HexGridManager.ResetClosestHexByPos(pos); }
		//Get Disc
		bool GetIsDiscValidByCoord(const glm::vec2& coord)const;
		glm::vec2 GetDiscPosByCoord(const glm::vec2& coord)const;
		glm::vec2 GetDiscTopPos()const;
		//Set Disc
		void SetDiscSteppedOn(const glm::vec2& coord)const;

		//General Setters
		void SetCurrentLevelID(int newID);
		void NextLevel();
		void ChangeHexColorByPos(const glm::vec2& pos);
		//
		void Render()const;
		void Update()const;
	private:
		friend class Singleton<LevelManager>;
		LevelManager();

		//LevelSettings
		int m_CurrentLevelID;
		static LevelSettings m_LevelSettings;
		//Grid
		static HexagonalGridManager m_HexGridManager;
		//Disc
		std::shared_ptr<Disc> m_Disc01;
		std::shared_ptr<Disc> m_Disc02;
	};
}

