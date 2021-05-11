#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class FontComponent;
	class LevelComponent final : public BaseComponent, public Observer
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

		void SetDisc01(const glm::vec2& coord);
		void SetDisc02(const glm::vec2& coord);

		void Notify(Event event) override;

		void NextLevel();

	private:
		int m_LevelID;
		bool m_NeedsUpdate;
		glm::vec2 m_Offset{ 550,50 };
		FontComponent* m_pFontComponent;
	};

}

