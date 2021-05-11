#pragma once
#include "Renderer.h"
#include <cmath>
#include <vector>
#include "Singleton.h"
#pragma warning(push)
#pragma warning (disable:4201)
//#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#pragma warning(pop)
#include "Texture2D.h"
#include "algorithm"
//some logic & code is sampled from https://www.redblobgames.com/grids/hexagons/implementation.html

struct Hex //putting this in the namespace will cause errors
{
    const int c;
    const int r;
    const int Radius{};
    int currentTexID;
    int maxStep;
    bool canGoBack;
    glm::vec2 pos{};
	
    Hex(int row, int column, const glm::vec2 origin, const int rad,int maxstep, bool can): c(column), r(row),Radius(rad), currentTexID(0)
	,maxStep(maxstep),canGoBack(can)
    {
 
        const glm::vec2 offset{ (glm::sin(60 * M_PI / 180) * 2) * Radius, Radius * 1.5f };
        glm::vec2 basePos{ origin.x + (offset.x * c) , origin.y + (offset.y * r) };
        if ((r % 2) != 0)
            basePos.x += offset.x / 2; //push odd rows by half the offset to avoid overlapping

        pos = basePos;
    }
    void changeTexID(int id) { currentTexID = id; }
    void nextStep()
    {
    	if(canGoBack)
    	{
    		if(currentTexID == maxStep)
    		{
                currentTexID = 0;
    		}
            else
            {
                ++currentTexID;
                currentTexID = std::min(currentTexID, maxStep);
            }
    	}
        else
        {
            ++currentTexID;
            currentTexID = std::min(currentTexID, maxStep);
        }

    }
	void SetNewLevel(int maxstep, bool can)
    {
        currentTexID = 0;
        maxStep = maxstep;
        canGoBack = can;
    }
};

namespace dae
{
    class HexagonalGridManager final
    {
    public:
        void loadFromFileRawHex(const std::string& file);
        void loadFromFileShape(const std::string& file);

        const glm::vec2 GetBaseGridPos() const { return m_GridBasePos; }
        const std::shared_ptr<Texture2D> GetHexTexture(int id) const;
        std::shared_ptr<Hex> getClosestHex(const glm::vec2& pos);
        int GetAmountOfSteps()const { return m_Amount; }

        const glm::vec2 GetHexPosByCoord(const glm::vec2& coord);
        const glm::vec2 GetCoordByClosestPos(const glm::vec2& pos);

        bool GetIsHexValidByCoord(const glm::vec2& coord);
        bool GetIsHexAlreadyFlippedByCoord(const glm::vec2& coord);

        void changeClosestHexByPos(const glm::vec2& pos);
        void changeClosestHexByCoord(const glm::vec2& coord);

        bool GetAreAllHexesFlipped();
    	
        void Render()
        {
            for (const auto& hex : m_HexGrid)
                Renderer::GetInstance().RenderHex(hex);
        }

        void SetNewLevel(int maxSteps, bool goBack);
    	
        void Init(int maxSteps, bool goBack);
    private:

        void addHex(Hex hex) { m_HexGrid.push_back(hex); }
    	
        std::vector<Hex> m_HexGrid;
        glm::vec2 m_GridBasePos{ 320,140 };
        int m_Radius{ 30 };
        int m_MaxSteps{};
        int m_Amount;
        bool m_CanChangeBack{};
        static const int MAX_HEX_TEXTURES = 3;
        std::shared_ptr<Texture2D> m_HexTextures[MAX_HEX_TEXTURES];
    };
}
