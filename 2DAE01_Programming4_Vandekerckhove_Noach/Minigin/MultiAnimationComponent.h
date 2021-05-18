#pragma once
#include "BaseComponent.h"

namespace dae
{
	enum class AnimState;

	class AnimationComponent;
	class CharacterComponent;
	class MultiAnimationComponent final : public BaseComponent
	{
	public:
		MultiAnimationComponent(AnimState state);
		~MultiAnimationComponent() override = default;

		MultiAnimationComponent(const MultiAnimationComponent& other) = delete;
		MultiAnimationComponent(MultiAnimationComponent&& other) noexcept = delete;
		MultiAnimationComponent& operator=(const MultiAnimationComponent& other) = delete;
		MultiAnimationComponent& operator=(MultiAnimationComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;
		void PostAddedToGameObject() override;

		void SetState(AnimState state, bool IsFlipped);
		
		void AddAnimationComponent(std::shared_ptr<AnimationComponent> comp);

		void SetAnimState(AnimState state) { m_State = state; }
		void SetFlipped(bool value, AnimState id);
		void SetFlippedCurrent(bool value);
		void SetPos(const glm::vec2& pos) { m_Pos = pos; m_NeedsUpdate = true; }
		void SetPos(const float x, const float y) { m_Pos.x = x; m_Pos.y = y; m_NeedsUpdate = true; }
	private:
		bool m_NeedsUpdate;
		glm::vec2 m_Pos{};
		AnimState m_State;
		CharacterComponent* m_Character;
		std::vector<std::shared_ptr<AnimationComponent>> m_AnimationComponents;
	};

}

