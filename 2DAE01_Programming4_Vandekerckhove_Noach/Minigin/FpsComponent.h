#pragma once
#include "BaseComponent.h"

namespace dae
{
	class FontComponent;
	class FpsComponent final : public BaseComponent
	{
	public:
		FpsComponent(const std::string& font, const unsigned int size);
		~FpsComponent() override;

		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) noexcept = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;

		void PostAddedToGameObject() override;

	private:
		FontComponent* m_pFontComponent;
	};
}


