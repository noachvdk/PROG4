#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <SDL_ttf.h>
#include "Texture2D.h"
#include "Font.h"
#include "GameObject.h"
#include "Transform.h"

using namespace dae;

TextureComponent::TextureComponent(const std::string& filename)
	:m_OffsetX{0}, m_OffsetY{0}
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

TextureComponent::~TextureComponent()
{

}

void TextureComponent::UpdateComponent()
{
}

void TextureComponent::RenderComponent()
{
	const auto pos = m_pParentObj->GetTransform().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x + m_OffsetX, pos.y + m_OffsetY);
}

void TextureComponent::SetOffset(float x, float y)
{
	m_OffsetX = x;
	m_OffsetY = y;
}