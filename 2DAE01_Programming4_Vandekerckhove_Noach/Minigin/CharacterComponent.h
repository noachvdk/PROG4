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

	class TextureComponent;
	class CharacterComponent final : public BaseComponent,public Observer
	{
	public:
		CharacterComponent(const std::string& filename);
		
		CharacterComponent(const CharacterComponent& other) = delete;
		CharacterComponent(CharacterComponent&& other) noexcept = delete;
		CharacterComponent& operator=(const CharacterComponent& other) = delete;
		CharacterComponent& operator=(CharacterComponent&& other) noexcept = delete;

		void move(direction dir);

		void UpdateComponent() override;
		void RenderComponent() override {};
		void PostAddedToGameObject() override;

		void Notify(Event event) override;
	private:
		direction m_MoveDirection;
		bool m_Move;
		bool m_Initialized;
		float m_MoveSpeed;
		float m_TextureSize;
		glm::vec2 m_StartPos;
		glm::vec2 m_CurrentPos;
		glm::vec2 m_NextPos;
		
		TextureComponent* m_pTextureComponent;
	};
}


