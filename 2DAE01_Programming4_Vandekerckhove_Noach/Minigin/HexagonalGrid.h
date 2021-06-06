#pragma once
#include <vector>
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
        HexagonalGridManager();
    	//load
        void loadFromFileRawHex(const std::string& file);
        void loadFromFileShape(const std::string& file);
        //Get general
        const int GetRadius() const { return m_Radius; }
        const glm::vec2 GetBaseGridPos() const { return m_GridBasePos; }
        const std::shared_ptr<Texture2D> GetHexTexture(int id) const;
        std::shared_ptr<Hex> getHexByPos(const glm::vec2& pos) const;
        std::shared_ptr<Hex> getHexByCoord(const glm::vec2& coord) const;
        int GetAmountOfSteps()const { return m_Amount; }
        //Get hex
        const glm::vec2 GetHexPosByCoord(const glm::vec2& coord) const;
        const glm::vec2 GetCoordByClosestPos(const glm::vec2& pos) const;
        bool GetIsHexValidByCoord(const glm::vec2& coord) const;
        bool GetIsHexAlreadyFlippedByCoord(const glm::vec2& coord) const;
        bool GetIsHexAlreadyFlippedByPos(const glm::vec2& pos) const;
    	std::vector<Hex> GetNeighboringHexesByCoord(const glm::vec2& coord) const;
        std::vector<Hex> GetNeighboringAccesibleHexesByCoord(const glm::vec2& coord) const;
        bool GetAreAllHexesFlipped();
        //Set
        void changeClosestHexByPos(const glm::vec2& pos);
        void changeClosestHexByCoord(const glm::vec2& coord);
        void ResetClosestHexByPos(const glm::vec2& pos);
        void SetAllHexColors(const int texID);

        void Render();

        void SetNewLevel(int maxSteps, bool goBack);
   
     
    private:
        void addHex(Hex hex) { m_HexGrid.push_back(hex); }
        void Init();
    	
        std::vector<Hex> m_HexGrid;
        glm::vec2 m_GridBasePos{};
        int m_Radius{ };
        int m_MaxSteps{};
        int m_Amount{};
        bool m_CanChangeBack{};
        static const int MAX_HEX_TEXTURES = 3;
        std::shared_ptr<Texture2D> m_HexTextures[MAX_HEX_TEXTURES];
    };
}
