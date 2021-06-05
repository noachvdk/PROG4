#pragma once
#include "PlayerControlledComponent.h"

namespace dae
{
	enum class direction;
	class MultiAnimationComponent;
	class CharacterComponent final : public PlayerControlledComponent/* BaseComponent,public Observer*/
	{
	public:
		CharacterComponent();
		~CharacterComponent() = default;
		CharacterComponent(const CharacterComponent& other) = delete;
		CharacterComponent(CharacterComponent&& other) noexcept = delete;
		CharacterComponent& operator=(const CharacterComponent& other) = delete;
		CharacterComponent& operator=(CharacterComponent&& other) noexcept = delete;

		void move(direction dir) override;

		glm::vec2 GetCurrentCharacterPos() const { return m_CurrentPos; }
		glm::vec2 GetDiscPos() const { return m_DiscPos; }
		bool GetOnDisc() const { return m_OnDisc; }

		void SetAnimComponent(MultiAnimationComponent* anim);
		
		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;
	private:
		//functions
		void Die();
		void CollisionWithPurpleEnemy();
		void CollisionWithGreenEnemy() const;
		
		//Movement
		direction m_MoveDirection;
		bool m_Move;
		bool m_Initialized;
		bool m_OnDisc;
		//Stunned
		bool m_isStunned;
		bool m_Switch;
		float m_StunTimer;
		float m_MaxStunTime;
		float m_StunSwitchTimer;
		float m_MaxStunSwitchTime;
		//Movement
		float m_MoveSpeed;
		float m_DiscMoveSpeed;
		float m_Distance;
		glm::vec2 m_StartPos;
		glm::vec2 m_CurrentPos;
		glm::vec2 m_NextPos;
		glm::vec2 m_CurrentCoord;
		glm::vec2 m_Origin;
		glm::vec2 m_DiscPos;
		//Disc move delay
		bool m_DiscDelay;
		float m_DiscDelayTime;
		float m_MaxDiscDelay;
		//Falling down
		bool m_IsFallingDown;
		float m_FallTimer;
		float m_MaxFallTime;
		glm::vec2 m_FallDownDir;
		//anim
		MultiAnimationComponent* m_Anim;
	};
}


