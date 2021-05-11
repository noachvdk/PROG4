#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class FontComponent;
	class LivesComponent final : public BaseComponent, public Observer
	{
	public:
		LivesComponent(const int startingLives, const std::string& font, const unsigned int size, bool displayHealth = false);
		~LivesComponent() override = default;

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) noexcept = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override{};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;
		
		int GetCurrentLives() const { return m_CurrentLives; }
		bool GetIsDead() const { return m_IsDead; }
		void AddTextOffset(const float x, const float y) const;

		void DecreaseHealth(int amount = 1);
	private:
		int m_CurrentLives;
		int m_MaxLives;
		bool m_IsDead;
		bool m_DisplayHealth;
		bool m_NeedsUpdate;
		FontComponent* m_pFontComponent;

	};
}

