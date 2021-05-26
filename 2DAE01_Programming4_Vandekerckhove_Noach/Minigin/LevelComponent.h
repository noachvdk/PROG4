#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class FontComponent;
	class LevelComponent final : public BaseComponent
	{
	public:
		LevelComponent(const int levelID);
		~LevelComponent() override = default;

		LevelComponent(const LevelComponent& other) = delete;
		LevelComponent(LevelComponent&& other) noexcept = delete;
		LevelComponent& operator=(const LevelComponent& other) = delete;
		LevelComponent& operator=(LevelComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;
		void PostAddedToGameObject() override;

		//void Notify(Event event) override;

		void NextLevel();

	private:
		int m_LevelID;
		bool m_NeedsUpdate;
		glm::vec2 m_Offset{ 550,50 }; //TextOffset
		FontComponent* m_pFontComponent;
	};

}

