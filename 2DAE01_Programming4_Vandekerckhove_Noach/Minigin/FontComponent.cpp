#include "MiniginPCH.h"
#include "FontComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <SDL_ttf.h>
#include "Texture2D.h"
#include "Font.h"
#include "GameObject.h"
#include "Transform.h"

using namespace dae;

FontComponent::FontComponent(const std::string& font, const unsigned int size, const std::string& text)
	: m_NeedsUpdate{true}
	, m_PosX{ 0.f }, m_PosY{ 0.f }
	, m_Color{ (Uint8)255.f, (Uint8)255.f, (Uint8)255.f, (Uint8)255.f }
	, m_Text{ text }
{
	m_pFont = ResourceManager::GetInstance().LoadFont(font, size);
	
	CreateTexture();
}

FontComponent::~FontComponent()
{
	safeDelete(m_pTexture);
	//safeDelete(m_pFont);
}

void FontComponent::CreateTexture()
{
	SDL_Surface* surface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	if (surface == nullptr)
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
	if (texture == nullptr)
		throw std::runtime_error(std::string("Create texture from surface failed: ") + SDL_GetError());
	SDL_FreeSurface(surface);

	safeDelete(m_pTexture);
	m_pTexture = new Texture2D(texture);
}

void FontComponent::UpdateComponent()
{
	if (m_NeedsUpdate)
	{
		CreateTexture();
		m_NeedsUpdate = false;

		if (m_pParentObj != nullptr)
		{
			m_PosX = m_OffsetX + m_pParentObj->GetTransform().GetPosition().x;
			m_PosY = m_OffsetY + m_pParentObj->GetTransform().GetPosition().y;
		}
	}
}

void FontComponent::RenderComponent()
{
	if (m_pTexture != nullptr)
		Renderer::GetInstance().RenderTexture(*m_pTexture, m_PosX, m_PosY);
}

void FontComponent::AddOffset(float x, float y)
{
	m_OffsetX = x;
	m_OffsetY = y;
	m_NeedsUpdate = true;
}

void FontComponent::SetColor(float r, float g, float b, float a)
{
	m_Color = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
	m_NeedsUpdate = true;
}

void FontComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void FontComponent::SetFont(const std::string& font, const unsigned int size)
{
	m_pFont = ResourceManager::GetInstance().LoadFont(font, size);
	m_NeedsUpdate = true;
}

