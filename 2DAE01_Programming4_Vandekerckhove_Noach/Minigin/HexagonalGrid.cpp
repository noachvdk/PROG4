#include "MiniginPCH.h"
#include "HexagonalGrid.h"
#include <fstream>
#include "Logger.h"
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
	const glm::vec2 coord{ getHexByPos(pos)->GetPos() };

	for(auto& hex : m_HexGrid)
	{
		if (coord == hex.GetPos())
			hex.NextTile();
	}
}

void HexagonalGridManager::changeClosestHexByCoord(const glm::vec2& coord)
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
			hex.NextTile();
	}
}

void HexagonalGridManager::ResetClosestHexByPos(const glm::vec2& pos)
{
	const glm::vec2 coord{ getHexByPos(pos)->GetPos() };
	
	for (auto& hex : m_HexGrid)
	{
		if (coord == hex.GetPos())
			hex.ResetTex();
	}
}

const glm::vec2 HexagonalGridManager::GetHexPosByCoord(const glm::vec2& coord) const
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
			return hex.GetPos();
	}
	return glm::vec2();
}

bool HexagonalGridManager::GetIsHexValidByCoord(const glm::vec2& coord) const
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
			return true;
	}
	return false;
}

bool HexagonalGridManager::GetIsHexAlreadyFlippedByCoord(const glm::vec2& coord) const
{
	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
		{
			if (hex.GetCurrentTexID() == hex.GetMaxSteps())
				return true;
		}

	}
	return false;
}

bool HexagonalGridManager::GetIsHexAlreadyFlippedByPos(const glm::vec2& pos) const
{
	const glm::vec2 coord{ getHexByPos(pos)->GetPos() };
	return GetIsHexAlreadyFlippedByCoord(coord);
}

std::vector<Hex> HexagonalGridManager::GetNeighboringHexesByCoord(const glm::vec2& coord) const
{
	std::vector<Hex> m_Neighbors{};
	for (auto hex : m_HexGrid)
	{
		if (coord.x == hex.r && coord.y == hex.c)
			continue;
		
		if(hex.r >= (coord.x - 1) && (coord.x + 1) >= hex.r)
		{
			if(coord.x == hex.r) //for the same row
			{
				if (hex.c >= coord.y - 1 && coord.y + 1 >= hex.c)
					m_Neighbors.push_back(hex);
			}
			else if ((int)coord.x % 2 == 0) //for the row above and under
			{
				if (hex.c >= coord.y - 1 && coord.y >= hex.c)
					m_Neighbors.push_back(hex);
			}
			else //for the row above and under
			{
				if (hex.c >= coord.y && coord.y + 1 >= hex.c)
					m_Neighbors.push_back(hex);
			}
			
		}
	}
	return m_Neighbors;
}

std::vector<Hex> HexagonalGridManager::GetNeighboringAccesibleHexesByCoord(const glm::vec2& coord) const
{
	std::vector<Hex> m_Neighbors{};
	for (auto hex : m_HexGrid)
	{
		if (coord.x == hex.r)
			continue;

		if (hex.r >= (coord.x - 1) && (coord.x + 1) >= hex.r)
		{
			if ((int)coord.x % 2 == 0) //for the row above and under
			{
				if (hex.c >= coord.y - 1 && coord.y >= hex.c)
					m_Neighbors.push_back(hex);
			}
			else //for the row above and under
			{
				if (hex.c >= coord.y && coord.y + 1 >= hex.c)
					m_Neighbors.push_back(hex);
			}

		}
	}
	return m_Neighbors;
}

const glm::vec2 HexagonalGridManager::GetCoordByClosestPos(const glm::vec2& pos) const
{
	const auto temp = getHexByPos(pos);
	return glm::vec2(temp->r, temp->c);
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
		if (hex.GetCurrentTexID() != hex.GetMaxSteps())
			return false;
	}
	
	return true;
}

void HexagonalGridManager::Render()
{
	for (const auto& hex : m_HexGrid)
		Renderer::GetInstance().RenderHex(hex);
}

void HexagonalGridManager::loadFromFileRawHex(const std::string& path)
{
	std::ifstream file{ dae::ResourceManager::GetInstance().GetFullPath() + path };
	std::string text;
	if (!file)
		Logger::GetInstance().Log(LogType::Error, "file not opened " + ResourceManager::GetInstance().GetFullPath() + path);

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
		Logger::GetInstance().Log(LogType::Error, "file not opened " + ResourceManager::GetInstance().GetFullPath() + path);

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
		Logger::GetInstance().Log(LogType::Warning, "no valid shape(currently only Square and Pyramid are supported");
	}
	//Mabye add more shapes
	Logger::GetInstance().Log(LogType::Info, "the level shape is " + text + " with size " + std::to_string(size));
	//file.close();
}

const std::shared_ptr<Texture2D> HexagonalGridManager::GetHexTexture(int id) const
{
	int i = std::min(id, MAX_HEX_TEXTURES - 1);
	i = std::max(i, 0);
	return m_HexTextures[i];
}

std::shared_ptr<Hex> dae::HexagonalGridManager::getHexByCoord(const glm::vec2& coord) const
{
	std::shared_ptr<Hex> currentHex = nullptr;

	for (auto& hex : m_HexGrid)
	{
		if (coord == glm::vec2(hex.r, hex.c))
			currentHex =std::make_shared<Hex>(hex);
	}

	return currentHex;
}

std::shared_ptr<Hex> HexagonalGridManager::getHexByPos(const glm::vec2& pos) const
{
	float distance{ float(INFINITE) };
	std::shared_ptr<Hex> currentHex = nullptr;
	
	for(auto& hex : m_HexGrid)
	{
		const auto currentDistance = glm::distance(pos, hex.GetPos());
		if(currentDistance<distance)
		{
			distance = currentDistance;
			currentHex = std::make_shared<Hex>(hex);
		}
	}
	
	return currentHex;
}
