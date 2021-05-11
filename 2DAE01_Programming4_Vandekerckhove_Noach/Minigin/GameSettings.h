#pragma once
#include "Singleton.h"

enum class GameMode : int
{
	NotChosen = 0,
	SinglePlayer = 1,
	Coop = 2,
	Versus = 3
};

namespace dae
{
	class GameObject;
	class GameSettings final : public Singleton<GameSettings>
	{
	public:

		void SetGameMode(GameMode newMode);
		void SetGameOver();
		GameMode GetGameMode() const { return m_CurrentGameMode; }

		void Init();
	private:
		friend class Singleton<GameSettings>;
		GameSettings();
		GameMode m_CurrentGameMode;		
		
	};
}

