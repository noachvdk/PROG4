#pragma once
#include "BaseComponent.h"
#include "FontComponent.h"
#include "Observer.h"

namespace dae
{
	class PointComponent final : public BaseComponent, public Observer
	{
	public:
		PointComponent(const std::string& font, const unsigned int size);
		~PointComponent() override = default;

		PointComponent(const PointComponent& other) = delete;
		PointComponent(PointComponent&& other) noexcept = delete;
		PointComponent& operator=(const PointComponent& other) = delete;
		PointComponent& operator=(PointComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void AddTextOffset(const float x,const float y) const;

		void Notify(Event event) override;

	private:
		bool m_NeedsUpdate;
		int m_CurrentPoints;
		FontComponent* m_pFontComponent;

		void AddPoints(const int points) { m_CurrentPoints += points; }
		void ResetPoints() { m_CurrentPoints = 0; }
	};
}

