#pragma once
#include "BaseComponent.h"
#include "CharacterComponent.h"

namespace dae
{
	class MultiAnimationComponent;
	class CharacterComponent;
	class SlickOrSamComponent final : public BaseComponent, public Observer
	{
	public:
		SlickOrSamComponent();
		~SlickOrSamComponent() = default;
		SlickOrSamComponent(const SlickOrSamComponent& other) = delete;
		SlickOrSamComponent(SlickOrSamComponent&& other) noexcept = delete;
		SlickOrSamComponent& operator=(const SlickOrSamComponent& other) = delete;
		SlickOrSamComponent& operator=(SlickOrSamComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override {};

		void Notify(Event event) override;
		
		void SetAnimComponent(MultiAnimationComponent* anim);
	private:
		void Die();
		direction RandomDirectionDown();
		void MoveDown();
		// variables
		bool m_Move;
		bool m_Initialized;
		//(re)spawing
		bool m_IsDead;
		float m_DeathTimer;
		float m_MaxDeathTime;
		//movement
		direction m_MoveDirection;
		float m_MoveSpeed;
		float m_MoveTimer;
		float m_MaxMoveTime;
		//pos
		glm::vec2 m_StartPos;
		glm::vec2 m_CurrentPos;
		glm::vec2 m_NextPos;
		//anim
		MultiAnimationComponent* m_Anim;
	};
}


