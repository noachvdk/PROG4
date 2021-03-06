#pragma once
#include "BaseComponent.h"

namespace dae
{
	class FontComponent;
	class LivesDisplayComponent final : public BaseComponent, public Observer
	{
	public:
		LivesDisplayComponent(const std::string& font, const unsigned int size);
		~LivesDisplayComponent() override = default;

		LivesDisplayComponent(const LivesDisplayComponent& other) = delete;
		LivesDisplayComponent(LivesDisplayComponent&& other) noexcept = delete;
		LivesDisplayComponent& operator=(const LivesDisplayComponent& other) = delete;
		LivesDisplayComponent& operator=(LivesDisplayComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;

		void AddTextOffset(const float x, const float y) const;
	private:
		FontComponent* m_pFontComponent;
	};
	
}

