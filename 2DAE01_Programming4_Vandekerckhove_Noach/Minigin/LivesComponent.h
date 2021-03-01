#pragma once
#include "BaseComponent.h"
#include "FontComponent.h"
#include "Observer.h"

namespace dae
{
	class LivesComponent final : public BaseComponent, public Observer
	{
	public:
		LivesComponent(const int startingLives, const std::string& font, const unsigned int size);
		~LivesComponent() override = default;

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) noexcept = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override{};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;

		void AddTextOffset(const float x, const float y) const;
		int GetCurrentLives() const { return m_CurrentLives; }
	private:
		int m_CurrentLives;
		int m_MaxLives;
		bool m_NeedsUpdate;
		FontComponent* m_pFontComponent;
		
	};
}

