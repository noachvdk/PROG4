#pragma once
#include "BaseComponent.h"
#include <string>
#include <SDL.h>

namespace dae
{
	class Texture2D;
	class GameObject;

	class TextureComponent final: public BaseComponent
	{
	public:
		TextureComponent(const std::string& filename);
		~TextureComponent() override;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) noexcept = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;
		void PostAddedToGameObject() override {};

		void SetOffset(float x, float y);
	private:
		float m_OffsetX, m_OffsetY;
		std::shared_ptr<Texture2D> m_pTexture;
	};

}

