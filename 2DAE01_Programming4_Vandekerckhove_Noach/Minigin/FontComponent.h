#pragma once
#include "BaseComponent.h"
#include <string>
#include <SDL.h>


namespace dae
{
	class Texture2D;
	class Font;
	class GameObject;

	class FontComponent final : public BaseComponent
	{
	public:
		FontComponent(const std::string& font, const unsigned int size, const std::string& text, bool moveWithParent = false);
		~FontComponent() override;

		FontComponent(const FontComponent& other) = delete;
		FontComponent(FontComponent&& other) noexcept = delete;
		FontComponent& operator=(const FontComponent& other) = delete;
		FontComponent& operator=(FontComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;
		void PostAddedToGameObject() override {};

		void SetText(const std::string& text);
		void SetFont(const std::string& font, const unsigned int size);
		void SetVisible(bool value) { m_Visible = value; }
		void AddOffset(float x, float y);
		void SetColor(float r, float g, float b, float a);
		void CreateTexture();
		
	private:
		bool m_NeedsUpdate;
		bool m_MovesWithParent;
		bool m_Visible;
		float m_PosX, m_PosY;
		float m_OffsetX, m_OffsetY;
		std::string m_Text;
		SDL_Color m_Color;
		Texture2D* m_pTexture;
		std::shared_ptr<Font> m_pFont;

	};
}


