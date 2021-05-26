#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace dae
{
    class CharacterComponent;

    enum class direction
    {
        UpLeft,
        UpRight,
        DownLeft,
        DownRight,
        Horizontal,
        Vertical,
    };
}


class Hex
{
public:
    const int c;
    const int r;
	
    Hex(int row, int column, const glm::vec2& origin, const int rad, int maxstep, bool can);
    void NextTile();
    void ResetTex();

	void SetNewLevel(int maxstep, bool can);
    void SetTexID(int id) { m_CurrentTexID = id; }
    void SetPlayerStandingOn(bool value) { m_StandingOn = value; }
    void SetPlayerStandingOn(dae::CharacterComponent* value) { m_Player = value; }

    int GetRadius() const { return m_Radius; }
    int GetCurrentTexID() const { return m_CurrentTexID; }
    int GetMaxSteps() const { return m_MaxStep; }
    bool GetCanGoBack() const { return m_CanGoBack; }
    bool GetIsPlayerStandingOn() const { return m_StandingOn; }
    glm::vec2 GetPos() const { return m_Pos; }
    dae::CharacterComponent* GetPlayerStandingOn() const { return m_Player; }
private:
    const int m_Radius{};
    int m_CurrentTexID;
    int m_MaxStep;
    bool m_CanGoBack;
    bool m_StandingOn;
    glm::vec2 m_Pos{};
    dae::CharacterComponent* m_Player;
	
};

