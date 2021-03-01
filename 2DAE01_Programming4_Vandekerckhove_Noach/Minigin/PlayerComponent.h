#pragma once
#include "BaseComponent.h"

namespace dae
{
	class PlayerComponent final : public BaseComponent
	{
	public:
		PlayerComponent(const unsigned int playerID);
		~PlayerComponent() override;

		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) noexcept = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override {};

		unsigned int GetPlayerID() const { return m_PlayerID; }
	private:
		const unsigned int m_PlayerID;
	};
}

