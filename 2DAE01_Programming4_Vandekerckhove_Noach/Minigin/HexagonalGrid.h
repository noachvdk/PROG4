#pragma once
#include "Renderer.h"
#include <vector>
#include "Singleton.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "Texture2D.h"
#include "Hex.h"
//some logic & code is sampled from https://www.redblobgames.com/grids/hexagons/implementation.html

namespace dae
{
    class CharacterComponent;
    class HexagonalGridManager final
    {
    public:
        void loadFromFileRawHex(const std::string& file);
        void loadFromFileShape(const std::string& file);

        const glm::vec2 GetBaseGridPos() const { return m_GridBasePos; }
        const std::shared_ptr<Texture2D> GetHexTexture(int id) const;
        std::shared_ptr<Hex> getHexByPos(const glm::vec2& pos) const;
        std::shared_ptr<Hex> getHexByCoord(const glm::vec2& coord) const;
        int GetAmountOfSteps()const { return m_Amount; }

        const glm::vec2 GetHexPosByCoord(const glm::vec2& coord) const;
        const glm::vec2 GetCoordByClosestPos(const glm::vec2& pos) const;

        bool GetIsHexValidByCoord(const glm::vec2& coord) const;
        bool GetIsHexAlreadyFlippedByCoord(const glm::vec2& coord) const;
        bool GetIsHexOccupiedByCoord(const glm::vec2& coord) const;
        bool GetIsHexOccupiedByPos(const glm::vec2& pos) const;
        CharacterComponent* GetCharacterOnHexByCoord(const glm::vec2& coord) const;
        CharacterComponent* GetCharacterOnHexByPos(const glm::vec2& pos) const;
    	std::vector<Hex> GetNeighboringHexesByCoord(const glm::vec2& coord) const;
        std::vector<Hex> GetNeighboringAccesibleHexesByCoord(const glm::vec2& coord) const;


    	void SetPlayerStandingOnHexByPos(const glm::vec2& pos,bool value);
        void SetPlayerStandingOnHexByPos(const glm::vec2& pos, CharacterComponent* value);

        void changeClosestHexByPos(const glm::vec2& pos);
        void changeClosestHexByCoord(const glm::vec2& coord);
        void ResetClosestHexByPos(const glm::vec2& pos);

        bool GetAreAllHexesFlipped();
    	
        void Render();

        void SetNewLevel(int maxSteps, bool goBack);
   
        void Init(int maxSteps, bool goBack);
    private:

        void addHex(Hex hex) { m_HexGrid.push_back(hex); }
    	
        std::vector<Hex> m_HexGrid;
        glm::vec2 m_GridBasePos{ 320,140 };
        int m_Radius{ 30 };
        int m_MaxSteps{};
        int m_Amount{};
        bool m_CanChangeBack{};
        static const int MAX_HEX_TEXTURES = 3;
        std::shared_ptr<Texture2D> m_HexTextures[MAX_HEX_TEXTURES];
    };
}
