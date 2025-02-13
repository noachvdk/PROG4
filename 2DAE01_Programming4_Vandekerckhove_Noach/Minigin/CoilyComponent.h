#pragma once
#include "PlayerControlledComponent.h"
namespace dae
{
	enum class direction;
	class MultiAnimationComponent;
	class CharacterComponent;
	class CoilyComponent final : public PlayerControlledComponent
	{
	public:
		CoilyComponent(bool playerControlled = false);
		~CoilyComponent() = default;
		CoilyComponent(const CoilyComponent& other) = delete;
		CoilyComponent(CoilyComponent&& other) noexcept = delete;
		CoilyComponent& operator=(const CoilyComponent& other) = delete;
		CoilyComponent& operator=(CoilyComponent&& other) noexcept = delete;

		glm::vec2 GetCurrentCharacterPos() const { return m_CurrentPos; }

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;

		void move(direction dir) override;

		void AddTarget(const CharacterComponent* target);
		void SetAnim(MultiAnimationComponent* anim);
	private:
		//Functions
		void TransformToSnake();
		void TransformToEgg();
		void Die();
		direction RandomDirectionDown();
		void SetRandomStartPos();
		void MoveDown();
		void MoveTowardsPlayer();
		
		// variables
		bool m_Move;
		bool m_Initialized;
		bool m_IsSnake;
		bool m_IsPlayerControlled;
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
		//Falling down
		bool m_IsFallingDown;
		float m_FallTimer;
		float m_MaxFallTime;
		glm::vec2 m_FallDownDir;
		//Target players
		std::vector<const CharacterComponent*> m_Targets;
		//anim
		MultiAnimationComponent* m_Anim;
	};

}

