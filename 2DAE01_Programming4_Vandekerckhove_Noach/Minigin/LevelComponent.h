#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class FontComponent;
	class LevelComponent final : public BaseComponent, public Observer
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

		void Notify(Event event) override;
		

	private:
		void NextLevel();
		
		int m_LevelID;
		glm::vec2 m_Offset{ 10,50 }; //TextOffset
		FontComponent* m_pFontComponent;
	};

}

