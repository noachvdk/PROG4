#pragma once
#include "BaseComponent.h"

namespace dae
{
	enum class AnimState;

	class AnimationComponent;
	class CharacterComponent;
	class MultiAnimationComponent final : public BaseComponent, public Observer
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

		void Notify(Event event) override;
		
		void AddAnimationComponent(std::shared_ptr<AnimationComponent> comp);

		void SetAnimState(AnimState state) { m_State = state; }
		void SetFlipped(bool value, AnimState id);
		void SetFlippedCurrent(bool value);
	private:
		AnimState m_State;
		CharacterComponent* m_Character;
		std::vector<std::shared_ptr<AnimationComponent>> m_AnimationComponents;
	};

}

