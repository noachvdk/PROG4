#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class FontComponent;
	class LevelComponent final : public BaseComponent
	{
	public:
		LevelComponent();
		~LevelComponent() override = default;

		LevelComponent(const LevelComponent& other) = delete;
		LevelComponent(LevelComponent&& other) noexcept = delete;
		LevelComponent& operator=(const LevelComponent& other) = delete;
		LevelComponent& operator=(LevelComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;
		void PostAddedToGameObject() override;

		void NextLevel();

	private:
		int m_LevelID;
		glm::vec2 m_Offset{ 550,50 }; //TextOffset
		FontComponent* m_pFontComponent;
	};

}

