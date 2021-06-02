#include "MiniginPCH.h"
#include "ColliderComponent.h"
#include "PlayerControlledComponent.h"
#include "Renderer.h"
#include "MultiAnimationComponent.h"
#include "AnimationComponent.h"
#include "Logger.h"
#include "CollisionManager.h"
#include "SubjectComponent.h"
#include "SlickOrSamComponent.h"
using namespace dae;

ColliderComponent::ColliderComponent(ColliderLayer layer)
	: m_Visualize(false)
	, m_Enabled(true)
	, m_Layer(layer)
	, m_Square()
	, m_Anim(nullptr)
{
	CollisionManager::GetInstance().AddCollider(this);
}

void ColliderComponent::UpdateComponent()
{
	if (m_Anim)
	{
		const auto temp = m_Anim->GetCurrentAnimComponent();
		if (temp)
			m_Square = temp->GetDestRect();
	}
}

void ColliderComponent::RenderComponent()
{
	if (m_Visualize && m_Anim && m_Anim->GetCurrentAnimState() != AnimState::Invisible)
		Renderer::GetInstance().RenderSquare(m_Square);
}

void ColliderComponent::CheckCollision(ColliderComponent* otherComp)
{
	//Several checks that may return early
	if (!m_Enabled || !otherComp->GetIsEnabled())
		return;
	
	if (m_pParentObj == otherComp->GetParentObject())
		return;

	if (m_Layer == otherComp->GetLayer())
		return;

	if (m_Anim && m_Anim->GetCurrentAnimState() == AnimState::Invisible)
		return;

	if (m_Layer != ColliderLayer::Friendly && otherComp->GetLayer() != ColliderLayer::Friendly)
		return;

	if (otherComp->GetAnim() && otherComp->GetAnim()->GetCurrentAnimState() == AnimState::Invisible)
		return;
	//Get squares
	const glm::vec2 pos = glm::vec2{ m_Square.x,m_Square.y };
	const int width = m_Square.w;
	const int height = m_Square.h;
	const auto otherSquare = otherComp->GetSquare();
	const glm::vec2 otherPos = glm::vec2{ otherSquare.x,otherSquare.y };
	const int otherWidth = otherSquare.w;
	const int otherHeight = otherSquare.h;

	if (otherPos == glm::vec2(0, 0) || pos == glm::vec2(0, 0))
		return;
	//check if collision
	if(otherPos.x < pos.x + width && otherPos.x + otherWidth > pos.x)
	{
		if (otherPos.y< pos.y + height && otherPos.y + otherHeight > pos.y)
		{
			Logger::GetInstance().Log(LogType::Info, "something hit");
			const auto temp = m_pParentObj->GetComponent<SubjectComponent>();
			if (temp && otherComp->GetLayer() == ColliderLayer::PurpleEnemy)
				temp->Notify(Event::ActorHitPurple);
			if (temp && otherComp->GetLayer() == ColliderLayer::GreenEnemy)
			{
				temp->Notify(Event::CatchedSlickOrSam);
				otherComp->GetParentObject()->GetComponent<SlickOrSamComponent>()->Notify(Event::ActorHitGreen);
			}
				
		}
	}
}

void dae::ColliderComponent::SetAnimComponent(MultiAnimationComponent* anim)
{
	m_Anim = anim;
}
