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

TextureComponent::TextureComponent(const std::string& filename, bool move, bool fixedsize)
	:m_OffsetX{0}, m_OffsetY{0}
	,m_MoveIndependently(move)
	,m_FixedSize(fixedsize)
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
	auto pos = m_pParentObj->GetTransform().GetPosition();
	if (m_MoveIndependently)
		pos = glm::vec3();

	if (m_FixedSize)
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x + m_OffsetX - (m_Size / 2), pos.y + m_OffsetY - (m_Size / 2), m_Size, m_Size);
	else
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x + m_OffsetX, pos.y + m_OffsetY);
}

void TextureComponent::SetOffset(float x, float y)
{
	m_OffsetX = x;
	m_OffsetY = y;
}