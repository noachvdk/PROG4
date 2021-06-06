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
		//Flash level when complete
		bool m_Flash;
		bool m_Switch;
		bool m_Victory;
		float m_FlashTimer;
		float m_MaxFlashTime;
		float m_FlashSwitchTimer;
		float m_MaxFlashSwitchTime;
		//Fonts
		glm::vec2 m_Offset{ 10,50 }; //Level: TextOffset
		FontComponent* m_pFontComponent;
		//Level completed
		bool m_LevelCompleted;
		glm::vec2 m_CompletedOffset{ 200,240 }; //Levelcompleted TextOffset
		FontComponent* m_pCompletedFontComponent;
	};

}

