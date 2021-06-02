#pragma once
#include "BaseComponent.h"
#include <SDL.h>

namespace dae
{
	class MultiAnimationComponent;

	enum class ColliderLayer
	{
		Friendly,
		PurpleEnemy,
		GreenEnemy,
	};
	
	class ColliderComponent final : public BaseComponent
	{
	public:
		ColliderComponent(ColliderLayer layer);
		~ColliderComponent() override = default;

		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) noexcept = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;
		void PostAddedToGameObject() override {};

		ColliderLayer GetLayer() const { return m_Layer; }
		SDL_Rect GetSquare() const { return m_Square; }
		MultiAnimationComponent* GetAnim() const { return m_Anim; }
		bool GetIsEnabled() const { return m_Enabled; }
		
		void CheckCollision(ColliderComponent* otherComp);

		void SetAnimComponent(MultiAnimationComponent* anim);
		void SetVisualize(bool value) { m_Visualize = value; }
		void SetDisabled() { m_Enabled = false; }
		void SetEnabled() { m_Enabled = true; }
		
	private:
		bool m_Visualize;
		bool m_Enabled;
		ColliderLayer m_Layer;
		SDL_Rect m_Square;
		//anim
		MultiAnimationComponent* m_Anim;
	};
}


