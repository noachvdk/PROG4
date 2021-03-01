#include "MiniginPCH.h"
#include "FpsComponent.h"
#include "TimeManager.h"
#include "FontComponent.h"

using namespace dae;

FpsComponent::FpsComponent(const std::string& font, const unsigned int size)
{
	m_pFontComponent = new FontComponent{ font,size," "};
	m_pFontComponent->SetColor(255.f, 0.f, 0.f, 255.f);
}

FpsComponent::~FpsComponent()
{
}

void FpsComponent::UpdateComponent()
{
	m_pFontComponent->SetText(std::to_string(TimeManager::GetInstance().GetFPS()) + " FPS");
}

void FpsComponent::RenderComponent()
{
	
}

void FpsComponent::PostAddedToGameObject()
{
	GetParentObject()->AddComponent(m_pFontComponent);
	m_pFontComponent->AddOffset(-GetParentObject()->GetTransform().GetPosition().x, -GetParentObject()->GetTransform().GetPosition().y);
}