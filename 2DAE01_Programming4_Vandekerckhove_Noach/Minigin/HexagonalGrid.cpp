#include "MiniginPCH.h"
#include "HexagonalGrid.h"
#include <iostream>
#include <fstream>
#include "ResourceManager.h"

using namespace dae;

void HexagonalGridManager::Init(int maxSteps, bool goBack)
{
	m_CanChangeBack = goBack;
	m_MaxSteps = maxSteps;
	m_Radius = 30;
	m_GridBasePos = { 320, 140 };
	m_HexTextures[0] = ResourceManager::GetInstance().LoadTexture("../Data/HexYellow.png");
	m_HexTextures[1] = ResourceManager::GetInstance().LoadTexture("HexPurple.png");
	m_HexTextures[2] = ResourceManager::GetInstance().LoadTexture("HexPink.png");
}

void HexagonalGridManager::changeClosestHexByPos(const glm::vec2& pos)
{
	float distance{ float(INFINITE) };
	glm::vec2 coord{};

	for (const auto& hex : m_HexGrid)
	{
		auto currentDistance = glm::distance(pos, hex.pos);
		if (currentDistance < distance)
		{
			distance = currentDistance;
			coord = hex.pos;
		}
	}

	for(auto& hex : m_HexGrid)
	{
		if (coord == hex.pos)
			hex.nextStep();
	}
}

void HexagonalGridManager::changeClosestHexByCoord(const glm::vec2& coord)
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
			hex.nextStep();
	}
}

const glm::vec2 HexagonalGridManager::GetHexPosByCoord(const glm::vec2& coord)
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
			return hex.pos;
	}
	return glm::vec2();
}

bool HexagonalGridManager::GetIsHexValidByCoord(const glm::vec2& coord)
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
			return true;
	}
	return false;
}

bool HexagonalGridManager::GetIsHexAlreadyFlippedByCoord(const glm::vec2& coord)
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
		{
			if (hex.currentTexID == hex.maxStep)
				return true;
		}

	}
	return false;
}

const glm::vec2 HexagonalGridManager::GetCoordByClosestPos(const glm::vec2& pos)
{
	return glm::vec2(getClosestHex(pos)->r, getClosestHex(pos)->c);
}

void HexagonalGridManager::SetNewLevel(int maxSteps, bool goBack)
{
	m_CanChangeBack = goBack;
	m_MaxSteps = maxSteps;
	for(auto& hex : m_HexGrid)
	{
		hex.SetNewLevel(m_MaxSteps, m_CanChangeBack);
	}
}

bool HexagonalGridManager::GetAreAllHexesFlipped()
{
	for (auto& hex : m_HexGrid)
	{
		if (hex.currentTexID != hex.maxStep)
			return false;
	}
	
	return true;
}

void HexagonalGridManager::loadFromFileRawHex(const std::string& path)
{
	std::ifstream file{ dae::ResourceManager::GetInstance().GetFullPath() + path };
	std::string text;
	if (!file)
		std::cout << "file not opened " << dae::ResourceManager::GetInstance().GetFullPath() + path << "\n";

	int x{};
	int y{};
	while (file >> x && file >> y)
	{
			//std::cout << x << " and " << y << "\n";
			addHex(Hex(x, y,m_GridBasePos, m_Radius,m_MaxSteps,m_CanChangeBack));
	}
	//file.close();
}

void HexagonalGridManager::loadFromFileShape(const std::string& path)
{
	std::ifstream file{ dae::ResourceManager::GetInstance().GetFullPath() + path };
	if (!file)
		std::cout << "file not opened " << dae::ResourceManager::GetInstance().GetFullPath() + path << "\n";

	std::string text;
	file >> text;
	int size{};
	file >> size;
	m_Amount = size;
	if(text == "Pyramid")
	{
		for (int r = 0; r <= size; r++)
		{
			for (int c = 0; c <= r ; c++)
			{
				if(r % 2 == 0)
				{
					if (r == 0)
						addHex(Hex(r , c, m_GridBasePos, m_Radius, m_MaxSteps, m_CanChangeBack));
					else
						addHex(Hex(r, c -(r/2), m_GridBasePos, m_Radius, m_MaxSteps, m_CanChangeBack));
				}
				else
				{
					addHex(Hex(r, c-(r/2)-1, m_GridBasePos, m_Radius, m_MaxSteps, m_CanChangeBack));
				}
			}
		}
	}
	else if (text == "Square")
	{
		for (int r = -size/2; r <= size/2+(size%2); r++)
		{
			for (int c = -size / 2; c <= size / 2 + (size % 2); c++)
			{
				addHex(Hex(r, c, m_GridBasePos, m_Radius, m_MaxSteps, m_CanChangeBack));
			}
		}
	}
	else
	{
		std::cout << "no valid shape (currently only Square and Pyramid are supported)\n";
	}
	//Mabye add more shapes
	std::cout <<"the shape is " <<text << " with size " << size << "\n";
	//file.close();
}

const std::shared_ptr<Texture2D> HexagonalGridManager::GetHexTexture(int id) const
{
	int i = std::min(id, MAX_HEX_TEXTURES - 1);
	i = std::max(i, 0);
	return m_HexTextures[i];
}

std::shared_ptr<Hex> HexagonalGridManager::getClosestHex(const glm::vec2& pos)
{
	float distance{ float(INFINITE) };
	std::shared_ptr<Hex> currentHex = std::make_shared<Hex>(0, 0, m_GridBasePos, m_Radius, m_MaxSteps, m_CanChangeBack);
	
	for(auto& hex : m_HexGrid)
	{
		auto currentDistance = glm::distance(pos, hex.pos);
		if(currentDistance<distance)
		{
			distance = currentDistance;
			currentHex = std::make_shared<Hex>(hex);

		}
	}
	
	return currentHex;
}
