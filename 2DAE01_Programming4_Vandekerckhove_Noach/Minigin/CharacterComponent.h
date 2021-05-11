#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	enum class direction
	{
		UpLeft,
		UpRight,
		DownLeft,
		DownRight
	};

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
		
		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;
	private:
		direction m_MoveDirection;
		bool m_Move;
		bool m_Initialized;
		bool m_OnDisc;
		bool m_IsDead;
		float m_MoveSpeed;
		float m_TextureSize;
		glm::vec2 m_StartPos;
		glm::vec2 m_CurrentPos;
		glm::vec2 m_NextPos;
		glm::vec2 m_CurrentCoord;
	};
}


