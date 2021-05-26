#pragma once
#include "BaseComponent.h"
#include "CharacterComponent.h"

namespace dae
{
	enum class Type : int
	{
		Ugg = 0,
		WrongWay = 1
	};
	
	class UggOrWrongwayComponent final : public BaseComponent, public Observer
	{
	public:
		UggOrWrongwayComponent(Type type);
		~UggOrWrongwayComponent() = default;
		UggOrWrongwayComponent(const UggOrWrongwayComponent& other) = delete;
		UggOrWrongwayComponent(UggOrWrongwayComponent&& other) noexcept = delete;
		UggOrWrongwayComponent& operator=(const UggOrWrongwayComponent& other) = delete;
		UggOrWrongwayComponent& operator=(UggOrWrongwayComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override {};

		void Notify(Event event) override;

		void SetAnimComponent(MultiAnimationComponent* anim);
	private:
		void Die();
		direction RandomDirection();
		void Move();
		glm::vec2 GetHexCollisionPos() const;
		// variables
		bool m_Move;
		//(re)spawing
		bool m_IsDead;
		float m_DeathTimer;
		float m_MaxDeathTime;
		//movement
		Type m_CurrentType;
		direction m_MoveDirection;
		float m_MoveSpeed;
		float m_MoveTimer;
		float m_MaxMoveTime;
		//pos
		glm::vec2 m_StartPos;
		glm::vec2 m_CurrentPos;
		glm::vec2 m_NextPos;
		//anim
		glm::vec2 m_Offset;
		MultiAnimationComponent* m_Anim;
	};

}

