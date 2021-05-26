#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	enum class direction;
	class MultiAnimationComponent;
	class CharacterComponent final : public BaseComponent,public Observer
	{
	public:
		CharacterComponent();
		~CharacterComponent() = default;
		CharacterComponent(const CharacterComponent& other) = delete;
		CharacterComponent(CharacterComponent&& other) noexcept = delete;
		CharacterComponent& operator=(const CharacterComponent& other) = delete;
		CharacterComponent& operator=(CharacterComponent&& other) noexcept = delete;

		void move(direction dir);

		glm::vec2 GetCurrentCharacterPos() const { return m_CurrentPos; }

		void SetAnimComponent(MultiAnimationComponent* anim);
		
		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;
		void CollisionWithPurpleEnemy();
		void CollisionWithGreenEnemy() const;
	private:
		direction m_MoveDirection;
		bool m_Move;
		bool m_Initialized;
		bool m_OnDisc;
		bool m_IsDead;
		//Stunned
		bool m_isStunned;
		bool m_Switch;
		float m_StunTimer;
		float m_MaxStunTime;
		float m_StunSwitchTimer;
		float m_MaxStunSwitchTime;
		//Movement
		float m_MoveSpeed;
		float m_Distance;
		glm::vec2 m_StartPos;
		glm::vec2 m_CurrentPos;
		glm::vec2 m_NextPos;
		glm::vec2 m_CurrentCoord;
		glm::vec2 m_MiddlePos;
		glm::vec2 m_Origin;
		//anim
		MultiAnimationComponent* m_Anim;
	};
}


