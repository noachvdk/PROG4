#pragma once
#include "BaseComponent.h"

namespace dae
{
	class LivesDisplayComponent;
	class LivesComponent final : public BaseComponent
	{
	public:
		LivesComponent(const int startingLives, bool displayHealth = false);
		~LivesComponent() override = default;

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) noexcept = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override{};
		void PostAddedToGameObject() override;

		int GetCurrentLives() const { return m_CurrentLives; }
		bool GetIsDead() const { return m_IsDead; }
		void AddTextOffset(const float x, const float y) const;

		void DecreaseHealth(int amount = 1);
		void Die();
	private:
		int m_CurrentLives;
		int m_MaxLives;
		bool m_IsDead;
		bool m_DisplayHealth;
		LivesDisplayComponent* m_pLivesDisplay;
	};
}

